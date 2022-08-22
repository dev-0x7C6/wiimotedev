#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote-ng.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include "include/wiimotedev/wiimotedev"
#include "containers/accelerometer-container.h"
#include "containers/infrared-container.h"
#include "containers/gyroscope-container.h"
#include "containers/pressure-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"
#include "interfaces/icontainer.h"

#include <spdlog/spdlog.h>

using namespace common::enums;
using namespace dae::api;
using namespace dae::container;
using namespace dae::container::structs;
using namespace dae::enums;
using namespace dae::interface;
using namespace wiimotedev;

static_assert(1ull << XWII_KEY_LEFT == WIIMOTEDEV_BTN_LEFT);
static_assert(1ull << XWII_KEY_RIGHT == WIIMOTEDEV_BTN_RIGHT);
static_assert(1ull << XWII_KEY_UP == WIIMOTEDEV_BTN_UP);
static_assert(1ull << XWII_KEY_DOWN == WIIMOTEDEV_BTN_DOWN);
static_assert(1ull << XWII_KEY_A == WIIMOTEDEV_BTN_A);
static_assert(1ull << XWII_KEY_B == WIIMOTEDEV_BTN_B);
static_assert(1ull << XWII_KEY_PLUS == WIIMOTEDEV_BTN_PLUS);
static_assert(1ull << XWII_KEY_MINUS == WIIMOTEDEV_BTN_MINUS);
static_assert(1ull << XWII_KEY_HOME == WIIMOTEDEV_BTN_HOME);
static_assert(1ull << XWII_KEY_ONE == WIIMOTEDEV_BTN_ONE);
static_assert(1ull << XWII_KEY_TWO == WIIMOTEDEV_BTN_TWO);
static_assert(1ull << XWII_KEY_X == WIIMOTEDEV_BTN_X);
static_assert(1ull << XWII_KEY_Y == WIIMOTEDEV_BTN_Y);
static_assert(1ull << XWII_KEY_TL == WIIMOTEDEV_BTN_TL);
static_assert(1ull << XWII_KEY_TR == WIIMOTEDEV_BTN_TR);
static_assert(1ull << XWII_KEY_ZL == WIIMOTEDEV_BTN_ZL);
static_assert(1ull << XWII_KEY_ZR == WIIMOTEDEV_BTN_ZR);
static_assert(1ull << XWII_KEY_THUMBL == WIIMOTEDEV_BTN_THUMBL);
static_assert(1ull << XWII_KEY_THUMBR == WIIMOTEDEV_BTN_THUMBR);
static_assert(1ull << XWII_KEY_C == WIIMOTEDEV_BTN_C);
static_assert(1ull << XWII_KEY_Z == WIIMOTEDEV_BTN_Z);

namespace {
template <typename type, typename input_type>
constexpr auto is_available(type &&flags, input_type &&match_with) noexcept {
	return (flags & match_with) == match_with;
}
}

XWiimoteController::XWiimoteController(IIdManager &manager, std::string &&path)
		: IWiimote(manager)
		, m_interfaceFilePath(std::move(path)) {
	setId(m_idManager.reserve(type()));
	m_buttons.fill(0);
	const auto wiiremote = spdlog::fmt_lib::format("wiiremote::{}", id());
	spdlog::debug("{} interface: {}", wiiremote, m_interfaceFilePath);

	if (openXWiimoteInterface() && watchXWiimoteEvents() && reconfigureXWiimoteInterface()) {
		m_connected = true;
	}
}

namespace helper {
struct xwii_iface_instance {
	xwii_iface_instance(xwii_iface **iface, const char *path)
			: iface(iface) {
		iface = (xwii_iface_new(iface, path) == 0) ? iface : nullptr;
	}

	constexpr auto valid() const noexcept -> bool {
		return iface != nullptr;
	}

	~xwii_iface_instance() {
		if (iface)
			xwii_iface_unref(*iface);
	}

private:
	xwii_iface **iface{nullptr};
};

struct xwii_iface_session {
	xwii_iface_session(xwii_iface *iface, const u32 flags)
			: iface(iface)
			, flags(flags) {
		iface = (xwii_iface_open(iface, flags) == 0) ? iface : nullptr;
	}

	constexpr auto valid() const noexcept -> bool {
		return iface != nullptr;
	}

	~xwii_iface_session() {
		if (iface)
			xwii_iface_close(iface, flags);
	}

private:
	xwii_iface *iface{nullptr};
	u32 flags{};
};
}

bool XWiimoteController::openXWiimoteInterface() {
	instance = std::make_unique<helper::xwii_iface_instance>(&m_interface, m_interfaceFilePath.c_str());

	if (!instance->valid()) {
		spdlog::error("unable to create {} interface", m_interfaceFilePath);
		return false;
	};

	return true;
}

bool XWiimoteController::watchXWiimoteEvents() {
	auto ret = xwii_iface_watch(m_interface, true);

	if (ret) {
		spdlog::error("fail: unable to attach watcher on interface");
		return false;
	}

	return true;
}

XWiimoteController::~XWiimoteController() {
	m_idManager.release(type(), id());
}

Device XWiimoteController::type() const {
	const auto flags = xwii_iface_available(m_interface);

	if (is_available(flags, XWII_IFACE_BALANCE_BOARD))
		return Device::BalanceBoard;

	if (is_available(flags, XWII_IFACE_PRO_CONTROLLER))
		return Device::ProController;

	return Device::Wiimote;
}

bool XWiimoteController::isValid() const { return m_connected; }

using Results = std::vector<std::unique_ptr<dae::interface::IContainer>>;

auto element(std::unique_ptr<dae::interface::IContainer> &&v) -> Results {
	Results ret;
	ret.emplace_back(std::move(v));
	return ret;
}

constexpr auto connection_state(const bool available) noexcept -> StatusContainer::State {
	return available ? StatusContainer::State::Connected : StatusContainer::State::Disconnected;
}

namespace process {
auto ir(const xwii_event &event) -> Results {
	IrPoints ir;
	for (std::size_t i = 0; i < ir.size(); ++i) {
		ir[i].x = event.v.abs[i].x;
		ir[i].y = event.v.abs[i].y;
		ir[i].size = xwii_event_ir_is_valid(&event.v.abs[i]) ? 1 : -1;
	}

	return element(std::make_unique<InfraredContainer>(ir));
}

auto acc(const Device source, const xwii_event &event, const int axis) -> Results {
	struct accdata data;
	data.x = event.v.abs[axis].x;
	data.y = event.v.abs[axis].y;
	data.z = event.v.abs[axis].z;
	data.roll = 0;
	data.pitch = 0;
	return element(std::make_unique<AccelerometerContainer>(source, data));
};

auto press(const xwii_event &event) {
	struct pressdata data;
	data.tl = event.v.abs[2].x;
	data.bl = event.v.abs[3].x;
	data.br = event.v.abs[1].x;
	data.tr = event.v.abs[0].x;
	return element(std::make_unique<PressureContainer>(data));
};

}

std::vector<std::unique_ptr<dae::interface::IContainer>> XWiimoteController::process() {
	auto process_key = [this](const Device source, const xwii_event &event) {
		auto mask = m_buttons.at(static_cast<std::size_t>(source));
		auto code = 1ull << event.v.key.code;
		static_assert(sizeof(code) == 8);

		if (event.v.key.state)
			mask |= code;
		else
			mask &= ~code;

		m_buttons.at(static_cast<std::size_t>(source)) = mask;
		return element(std::make_unique<ButtonContainer>(source, mask));
	};

	const auto wiiremote = spdlog::fmt_lib::format("wiiremote::{}", id());

	auto process_gone = [&]() -> Results {
		spdlog::debug("{} gone", wiiremote);

		Results ret;
		if (m_nunchukConnected.value_or(false)) {
			ret.emplace_back(std::make_unique<StatusContainer>(Device::Nunchuk, StatusContainer::State::Disconnected));
			spdlog::debug("{} report nunchuk: {}", wiiremote, "disconnected");
		}

		if (m_classicControllerConnected.value_or(false)) {
			ret.emplace_back(std::make_unique<StatusContainer>(Device::Classic, StatusContainer::State::Disconnected));
			spdlog::debug("{} report classic controller: {}", wiiremote, "disconnected");
		}

		if (m_balanceBoardConnected.value_or(false)) {
			ret.emplace_back(std::make_unique<StatusContainer>(Device::BalanceBoard, StatusContainer::State::Disconnected));
			spdlog::debug("{} report balance board: {}", wiiremote, "disconnected");
		}

		if (m_proControllerConnected.value_or(false)) {
			ret.emplace_back(std::make_unique<StatusContainer>(Device::ProController, StatusContainer::State::Disconnected));
			spdlog::debug("{} report pro controller: {}", wiiremote, "disconnected");
		}

		if (m_wiimoteConnected.value_or(false)) {
			ret.emplace_back(std::make_unique<StatusContainer>(Device::Wiimote, StatusContainer::State::Disconnected));
			spdlog::debug("{} report wiiremote: {}", wiiremote, "disconnected");
		}

		m_connected = false;
		return ret;
	};

	auto process_stick = [](Device device, const xwii_event &event) {
		auto calculate = [](i32 &x, i32 &y, i32 step) {
			constexpr i32 min = WIIMOTEDEV_STICK_MIN;
			constexpr i32 max = WIIMOTEDEV_STICK_MAX;
			x = std::max(-step, std::min(step, x)) * (max / step);
			y = std::max(-step, std::min(step, y)) * (max / step);
			x = std::max(min, std::min(max, x));
			y = std::max(min, std::min(max, y));
		};

		i32 lx = event.v.abs[0].x;
		i32 ly = event.v.abs[0].y;
		i32 rx = event.v.abs[1].x;
		i32 ry = event.v.abs[1].y;

		if (device == Device::Nunchuk) {
			calculate(lx, ly, 100);
			return element(std::make_unique<StickContainer>(device, lx, ly, 0, 0));
		}

		if (device == Device::Classic) {
			calculate(lx, ly, 24);
			calculate(rx, ry, 24);
			return element(std::make_unique<StickContainer>(device, lx, ly, rx, ry));
		}

		if (device == Device::ProController) {
			calculate(lx, ly, 1280);
			calculate(rx, ry, 1280);
			ly *= -1;
			ry *= -1;
			return element(std::make_unique<StickContainer>(device, lx, ly, rx, ry));
		}

		return element(std::make_unique<StickContainer>(device,
			event.v.abs[0].x, event.v.abs[0].y, event.v.abs[1].x, event.v.abs[1].y));
	};

	auto process_watch = [this]() -> Results {
		reconfigureXWiimoteInterface();
		return {};
	};

	struct xwii_event event;
	event.type = XWII_EVENT_NUM;

	u32 i = 0;

	Results results;
	while (xwii_iface_dispatch(m_interface, &event, sizeof(event)) == 0) {
		i++;
		auto x = [&]() -> Results {
			switch (event.type) {
				case XWII_EVENT_ACCEL: return process::acc(Device::Wiimote, event, 0);
				case XWII_EVENT_BALANCE_BOARD: return process::press(event);
				case XWII_EVENT_CLASSIC_CONTROLLER_KEY: return process_key(Device::Classic, event);
				case XWII_EVENT_CLASSIC_CONTROLLER_MOVE: return process_stick(Device::Classic, event);
				case XWII_EVENT_PRO_CONTROLLER_KEY: return process_key(Device::ProController, event);
				case XWII_EVENT_PRO_CONTROLLER_MOVE: return process_stick(Device::ProController, event);
				case XWII_EVENT_GONE: return process_gone();
				case XWII_EVENT_IR: return process::ir(event);
				case XWII_EVENT_KEY: return process_key(Device::Wiimote, event);
				case XWII_EVENT_MOTION_PLUS: return element(std::make_unique<GyroscopeContainer>(event.v.abs[0].x, event.v.abs[0].y, event.v.abs[0].z));
				case XWII_EVENT_NUNCHUK_KEY: return process_key(Device::Nunchuk, event);
				case XWII_EVENT_NUNCHUK_MOVE: {
					Results ret;
					ret.emplace_back(std::move(process::acc(Device::Nunchuk, event, 1).front()));
					ret.emplace_back(std::move(process_stick(Device::Nunchuk, event).front()));
					return ret;
				}

				case XWII_EVENT_WATCH: return process_watch();
			}

			return {};
		}();
		std::move(x.begin(), x.end(), std::back_inserter(results));
	}

	if (i != 0)
		spdlog::info("{}, readed times: {}", wiiremote, i);

	return results;
}

bool XWiimoteController::isRumbleSupported() {
	return true;
}

bool XWiimoteController::isLedSupported() {
	return true;
}

bool XWiimoteController::isInfraredSupported() {
	return is_available(xwii_iface_available(m_interface), XWII_IFACE_IR);
}

u8 XWiimoteController::batteryStatus() {
	u8 capacity = 0;
	xwii_iface_get_battery(m_interface, &capacity);
	return capacity;
}

bool XWiimoteController::ledStatus(const u32 id) {
	bool state = false;
	xwii_iface_get_led(m_interface, id, &state);
	return state;
}

bool XWiimoteController::rumbleStatus() {
	return m_rumbleStatus;
}

bool XWiimoteController::setLedStatus(const u32 id, const bool status) {
	return xwii_iface_set_led(m_interface, id, status) == 0;
}

bool XWiimoteController::setRumbleStatus(const bool rumble) {
	const auto isValid = xwii_iface_rumble(m_interface, rumble) == 0;
	if (isValid)
		m_rumbleStatus = rumble;
	return isValid;
}

bool XWiimoteController::hasClassicExtension() {
	return is_available(xwii_iface_available(m_interface), XWII_IFACE_CLASSIC_CONTROLLER);
}

bool XWiimoteController::hasMotionPlusExtension() {
	return is_available(xwii_iface_available(m_interface), XWII_IFACE_MOTION_PLUS);
}

bool XWiimoteController::hasNunchukExtension() {
	return is_available(xwii_iface_available(m_interface), XWII_IFACE_NUNCHUK);
}

std::string XWiimoteController::interfaceFilePath() const {
	return m_interfaceFilePath;
}

bool XWiimoteController::reconfigureXWiimoteInterface() {
	auto flags = xwii_iface_available(m_interface) | XWII_IFACE_WRITABLE;
	session = std::make_unique<helper::xwii_iface_session>(m_interface, flags);

	const auto wiiremote = spdlog::fmt_lib::format("wiiremote::{}", id());
	spdlog::debug("process flags");

	Results ret;

	if (const auto available = is_available(flags, XWII_IFACE_CORE); available != m_wiimoteConnected) {
		m_wiimoteConnected = available;
		ret.emplace_back(std::make_unique<StatusContainer>(Device::Wiimote, connection_state(available)));
		spdlog::debug("{} report wiiremote: {}", wiiremote, available ? "connected" : "disconnected");
	}

	if (const auto available = is_available(flags, XWII_IFACE_NUNCHUK); available != m_nunchukConnected) {
		m_nunchukConnected = available;
		ret.emplace_back(std::make_unique<StatusContainer>(Device::Nunchuk, connection_state(available)));
		spdlog::debug("{} report nunchuk: {}", wiiremote, available ? "connected" : "disconnected");
	}

	if (const auto available = is_available(flags, XWII_IFACE_CLASSIC_CONTROLLER); available != m_classicControllerConnected) {
		m_classicControllerConnected = available;
		ret.emplace_back(std::make_unique<StatusContainer>(Device::Classic, connection_state(available)));
		spdlog::debug("{} report classic controller: {}", wiiremote, available ? "connected" : "disconnected");
	}

	if (const auto available = is_available(flags, XWII_IFACE_MOTION_PLUS); available != m_motionPlusConnected) {
		m_motionPlusConnected = available;
		// m_messages.emplace(std::make_unique<StatusContainer>(Device::, connection_state(available)));
		spdlog::debug("{} report motion+: {}", wiiremote, available ? "connected" : "disconnected");
	}

	if (const auto available = is_available(flags, XWII_IFACE_BALANCE_BOARD); available != m_balanceBoardConnected) {
		m_balanceBoardConnected = available;
		ret.emplace_back(std::make_unique<StatusContainer>(Device::BalanceBoard, connection_state(available)));
		spdlog::debug("{} report balance board: {}", wiiremote, available ? "connected" : "disconnected");
	}

	if (const auto available = is_available(flags, XWII_IFACE_PRO_CONTROLLER); available != m_proControllerConnected) {
		m_proControllerConnected = available;
		ret.emplace_back(std::make_unique<StatusContainer>(Device::ProController, connection_state(available)));
		spdlog::debug("{} report pro controller: {}", wiiremote, available ? "connected" : "disconnected");
	}

	auto is_reporting = [&](const u32 match) -> bool {
		return (flags & match) != 0;
	};

	spdlog::debug("{} report table:", wiiremote);
	spdlog::debug("{}   -> core:           {}", wiiremote, is_reporting(XWII_IFACE_CORE));
	spdlog::debug("{}   -> accelerometer:  {}", wiiremote, is_reporting(XWII_IFACE_ACCEL));
	spdlog::debug("{}   -> motion+:        {}", wiiremote, is_reporting(XWII_IFACE_MOTION_PLUS));
	spdlog::debug("{}   -> nunchuk:        {}", wiiremote, is_reporting(XWII_IFACE_NUNCHUK));
	spdlog::debug("{}   -> classic:        {}", wiiremote, is_reporting(XWII_IFACE_CLASSIC_CONTROLLER));
	spdlog::debug("{}   -> balance board:  {}", wiiremote, is_reporting(XWII_IFACE_BALANCE_BOARD));
	spdlog::debug("{}   -> pro controller: {}", wiiremote, is_reporting(XWII_IFACE_PRO_CONTROLLER));

	if (!session->valid()) {
		spdlog::error("{} session interface is broken", wiiremote);
		m_connected = false;
		return false;
	}

	return true;
}
