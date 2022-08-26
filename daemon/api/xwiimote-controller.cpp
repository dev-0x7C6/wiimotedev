#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote-ng.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>

#include "include/wiimotedev/wiimotedev"
#include "containers/structs.hpp"

#include <spdlog/spdlog.h>

using namespace std::chrono_literals;
using namespace common::enums;
using namespace dae::api;
using namespace dae::container;
using namespace dae::container;
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
			: iface(iface) {
		std::this_thread::sleep_for(300ms);

		if (const auto ret = xwii_iface_open(iface, flags | XWII_IFACE_WRITABLE); ret) {
			spdlog::error("xwii_iface_open: unable to open inteface: ret: {}", ret);
			iface = nullptr;
		}
	}

	constexpr auto valid() const noexcept -> bool {
		return iface != nullptr;
	}

	~xwii_iface_session() {
		if (iface)
			xwii_iface_close(iface, XWII_IFACE_ALL);
	}

private:
	xwii_iface *iface{nullptr};
};
}

constexpr auto to_xwii_interface(const device v) noexcept -> u32 {
	switch (v) {
		case device::wiimote: return XWII_IFACE_CORE;
		case device::nunchuk: return XWII_IFACE_NUNCHUK;
		case device::classic_controller: return XWII_IFACE_CLASSIC_CONTROLLER;
		case device::pro_controller: return XWII_IFACE_PRO_CONTROLLER;
		case device::motion_plus: return XWII_IFACE_MOTION_PLUS;
		case device::balance_board: return XWII_IFACE_BALANCE_BOARD;
	}

	return XWII_IFACE_CORE;
};

template <typename T, typename... args>
void reconstruct(std::unique_ptr<T> &interface, args &&...values) {
	interface = nullptr;
	interface = std::make_unique<T>(std::forward<args>(values)...);
}

bool XWiimoteController::openXWiimoteInterface() {
	reconstruct(instance, &m_interface, m_interfaceFilePath.c_str());

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

device XWiimoteController::type() const {
	const auto flags = xwii_iface_available(m_interface);

	if (is_available(flags, XWII_IFACE_BALANCE_BOARD))
		return device::balance_board;

	if (is_available(flags, XWII_IFACE_PRO_CONTROLLER))
		return device::pro_controller;

	return device::wiimote;
}

bool XWiimoteController::isValid() const { return m_connected; }

namespace process {
auto ir(const xwii_event &event) -> dae::container::event {
	dae::container::ir_points ret;
	for (auto i = 0u; i < ret.size(); ++i) {
		ret[i].x = event.v.abs[i].x;
		ret[i].y = event.v.abs[i].y;
		ret[i].size = xwii_event_ir_is_valid(&event.v.abs[i]) ? 1 : -1;
	}

	return {common::enums::device::wiimote, ret};
}

auto gyro(const xwii_event &event) -> dae::container::event {
	dae::container::gyro ret;
	ret.x = event.v.abs[0].x;
	ret.y = event.v.abs[0].y;
	ret.z = event.v.abs[0].z;
	return std::make_pair(common::enums::device::wiimote, std::move(ret));
}

auto acc(const device source, const xwii_event &event, const int axis) -> dae::container::event {
	dae::container::accdata ret;
	ret.x = event.v.abs[axis].x;
	ret.y = event.v.abs[axis].y;
	ret.z = event.v.abs[axis].z;
	ret.roll = 0.0;
	ret.pitch = 0.0;
	return std::make_pair(source, std::move(ret));
};

auto press(const xwii_event &event) -> dae::container::event {
	dae::container::pressure ret;
	ret.top = {event.v.abs[2].x, event.v.abs[0].x};
	ret.bottom = {event.v.abs[3].x, event.v.abs[1].x};
	return std::make_pair(common::enums::device::balance_board, ret);
};
}

events XWiimoteController::process() {
	auto process_key = [this](const device source, const xwii_event &event) {
		auto mask = m_buttons.at(static_cast<std::size_t>(source));
		auto code = 1ull << event.v.key.code;
		static_assert(sizeof(code) == 8);

		if (event.v.key.state)
			mask |= code;
		else
			mask &= ~code;

		m_buttons.at(static_cast<std::size_t>(source)) = mask;
		dae::container::button ret;
		ret.states = mask;
		return std::make_pair(source, std::move(ret));
	};

	const auto wiiremote = spdlog::fmt_lib::format("wiiremote::{}", id());

	auto process_gone = [&]() -> dae::container::events {
		dae::container::events ret;

		currentExtensionTable.fill(false);

		for (auto &&dev : devices) {
			const auto idx = static_cast<std::size_t>(dev);
			if (currentExtensionTable[idx] == lastExtensionTable[idx])
				continue;

			const auto available = currentExtensionTable[idx];
			dae::container::status status;
			status.is_connected = available;
			ret.emplace_back(std::make_pair(common::enums::device::wiimote, std::move(status)));
			spdlog::debug("{} report {}: {}", wiiremote, to_string(dev), available ? "connected" : "disconnected");
		}

		lastExtensionTable = currentExtensionTable;

		m_connected = false;
		return ret;
	};

	auto process_stick = [](device device, const xwii_event &event) -> dae::container::event {
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

		if (is::nunchuk(device)) {
			calculate(lx, ly, 100);
			return std::make_pair(device, dae::container::stick{lx, ly});
		}

		if (is::classic_controller(device)) {
			calculate(lx, ly, 24);
			calculate(rx, ry, 24);
			return std::make_pair(device, dae::container::stick_pair{{lx, ly}, {rx, ry}});
		}

		if (is::pro_controller(device)) {
			calculate(lx, ly, 1280);
			calculate(rx, ry, 1280);
			ly *= -1;
			ry *= -1;
			return std::make_pair(device, dae::container::stick_pair{{lx, ly}, {rx, ry}});
		}

		return std::make_pair(device, dae::container::stick_pair{{lx, ly}, {rx, ry}});
	};

	auto process_watch = [this]() {
		reconfigureXWiimoteInterface();
	};

	dae::container::events ret;
	for (;;) {
		xwii_event event{};
		event.type = XWII_EVENT_NUM;

		if (xwii_iface_dispatch(m_interface, &event, sizeof(event)))
			break;

		auto events = [&]() -> dae::container::events {
			switch (event.type) {
				case XWII_EVENT_ACCEL: return {process::acc(device::wiimote, event, 0)};
				case XWII_EVENT_BALANCE_BOARD: return {process::press(event)};
				case XWII_EVENT_CLASSIC_CONTROLLER_KEY: return {process_key(device::classic_controller, event)};
				case XWII_EVENT_CLASSIC_CONTROLLER_MOVE: return {process_stick(device::classic_controller, event)};
				case XWII_EVENT_PRO_CONTROLLER_KEY: return {process_key(device::pro_controller, event)};
				case XWII_EVENT_PRO_CONTROLLER_MOVE: return {process_stick(device::pro_controller, event)};
				case XWII_EVENT_GONE: process_gone(); break;
				case XWII_EVENT_IR: return {process::ir(event)};
				case XWII_EVENT_KEY: return {process_key(device::wiimote, event)};
				case XWII_EVENT_MOTION_PLUS: return {process::gyro(event)};
				case XWII_EVENT_NUNCHUK_KEY:
					return {process_key(device::nunchuk, event)};

				case XWII_EVENT_NUNCHUK_MOVE:
					return {
						process::acc(device::nunchuk, event, 1),
						process_stick(device::nunchuk, event)};

				case XWII_EVENT_WATCH:
					process_watch();
					break;
			}

			return {};
		}();
		std::move(events.begin(), events.end(), std::back_inserter(ret));
	}

	return ret;
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
	return xwii_iface_set_led(m_interface, id, status);
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
	const auto wiiremote = spdlog::fmt_lib::format("wiiremote::{}", id());
	const auto flags = xwii_iface_available(m_interface);

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

	if (xwii_iface_opened(m_interface) == flags) {
		spdlog::debug("nothing to do");
		return true;
	}

	reconstruct(session, m_interface, flags);

	dae::container::events ret;

	for (auto &&dev : devices)
		currentExtensionTable[static_cast<std::size_t>(dev)] = is_available(flags, to_xwii_interface(dev));

	for (auto &&dev : devices) {
		const auto idx = static_cast<std::size_t>(dev);
		if (currentExtensionTable[idx] == lastExtensionTable[idx])
			continue;

		const auto available = currentExtensionTable[idx];
		dae::container::status status;
		status.is_connected = available;
		ret.emplace_back(std::make_pair(common::enums::device::wiimote, std::move(status)));
		spdlog::debug("{} report {}: {}", wiiremote, to_string(dev), available ? "connected" : "disconnected");
	}

	lastExtensionTable = currentExtensionTable;

	if (!session->valid()) {
		spdlog::error("{} session interface is broken", wiiremote);
		m_connected = false;
		return false;
	}

	return true;
}
