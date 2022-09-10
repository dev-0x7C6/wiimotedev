#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote-ng.h>

#include <algorithm>
#include <numeric>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <thread>

#include "include/wiimotedev/wiimotedev"
#include "containers/structs.hpp"

#include <fmt/format.h>
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

namespace debug::gyroscope {
constexpr auto visible = false;
}

namespace debug::accelerometer {
constexpr auto visible = false;
}

namespace {
template <typename type, typename input_type>
constexpr auto is_available(type &&flags, input_type &&match_with) noexcept {
	return (flags & match_with) == match_with;
}
}

XWiimoteController::XWiimoteController(IIdManager &manager, std::string &&path)
		: IWiimote(manager)
		, m_interfaceFilePath(std::move(path)) {
	if (openXWiimoteInterface() && watchXWiimoteEvents() && reconfigureXWiimoteInterface()) {
		m_connected = true;
	}

	setId(m_idManager.reserve(type()));
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
		ret[i].valid = xwii_event_ir_is_valid(&event.v.abs[i]);
	}

	return {common::enums::device::wiimote, ret};
}

namespace event {
constexpr auto to_gyro(const xwii_event &event, const s32 axis = 0) noexcept -> dae::container::gyro {
	dae::container::gyro ret;
	ret.axies = {
		.x = static_cast<double>(event.v.abs[axis].x),
		.y = static_cast<double>(event.v.abs[axis].y),
		.z = static_cast<double>(event.v.abs[axis].z),
	};

	return ret;
}

constexpr auto to_usecs(const xwii_event &event) noexcept -> std::chrono::microseconds {
	std::chrono::microseconds ret(event.time.tv_usec);
	if (event.time.tv_sec >= 0)
		ret += std::chrono::seconds(event.time.tv_sec);
	return ret;
}

constexpr auto time_delta_sec(const xwii_event &current, const std::chrono::microseconds last) noexcept -> double {
	return (last - to_usecs(current)).count() / 1'000'000.0;
}
}

namespace filter {
constexpr auto high_pass(const dae::container::axis3d &current, const dae::container::axis3d &prev, const double alpha = 0.99) {
	dae::container::axis3d ret = current;
	ret *= alpha;
	ret += prev * (1.0 - alpha);
	return ret;
}
}

auto gyro(accel_state_cache &acc, gyro_state_cache &cache, const xwii_event &event) -> dae::container::event {
	if (!cache.last) {
		cache.last = event::to_usecs(event);
		return {};
	}

	const auto dt = event::time_delta_sec(event, cache.last.value());
	auto current = event::to_gyro(event);
	cache.last = event::to_usecs(event);

	if (!cache.callibration.result) {
		auto &&probes = cache.callibration.probes;
		probes.emplace_back(current.axies);
		std::rotate(probes.rbegin(), probes.rbegin() + 1, probes.rend());
		probes[0] = current.axies;

		if (cache.callibration.preffered_probe_count > probes.size())
			return {};

		cache.callibration.result = std::accumulate(probes.begin(), probes.end(), dae::container::axis3d{}) / probes.size();
		probes.clear();
	}

	// align values to zero
	current.axies -= cache.callibration.result.value();

	// multiply by delta time and divide by xwii_gyro_linear_scale to get deg/s
	constexpr auto degree_per_sec_constant = 8192.0 * 9.0 / 440.0; // 167.563~
	current.axies /= degree_per_sec_constant / dt;

	// high pass filter
	if (cache.prev)
		current.axies = filter::high_pass(current.axies, cache.prev.value(), 0.98);

	// accumulate with previous reading
	cache.processed.axies += current.axies;

	if (acc.stability_score < 1.01) {
		cache.processed.axies.y = cache.processed.axies.y * 0.98 + acc.prev.raw.y * 0.02;
		cache.processed.axies.z = cache.processed.axies.z * 0.98 + acc.prev.raw.z * -0.02;
	}

	// save as prev sample
	cache.prev = current.axies;

	if constexpr (debug::gyroscope::visible) {
		const auto raw = event::to_gyro(event);
		spdlog::debug("gyroscope:");
		spdlog::debug("  ----------------------");
		spdlog::debug("     raw: [x]: {:+.0f}", raw.axies.x);
		spdlog::debug("     raw: [y]: {:+.0f}", raw.axies.y);
		spdlog::debug("     raw: [z]: {:+.0f}", raw.axies.z);
		spdlog::debug("  ----------------------");
		spdlog::debug("    yaw:  [x]: {:+.3f}°", cache.processed.axies.x);
		spdlog::debug("   roll:  [y]: {:+.3f}°", cache.processed.axies.y);
		spdlog::debug("  pitch:  [z]: {:+.3f}°", cache.processed.axies.z);
		spdlog::debug("   time: [Δt]: {:+.3f}s", dt);
	}

	return std::make_pair(common::enums::device::wiimote, std::move(cache.processed));
}

auto acc(accel_state_cache &cache, const device source, const xwii_event &event, const int axis) -> dae::container::event {
	dae::container::accdata ret{};
	ret.raw.x = event.v.abs[axis].x + 26;
	ret.raw.y = event.v.abs[axis].y + 26;
	ret.raw.z = event.v.abs[axis].z + 26;

	if (!cache.last) {
		cache.last = event::to_usecs(event);
		return {};
	}

	const auto dt = event::time_delta_sec(event, cache.last.value());
	cache.last = event::to_usecs(event);

	auto &&probes = cache.probes;

	if (probes.size() < 32)
		probes.emplace_back(ret.raw);

	if (probes.size() >= 32) {
		std::rotate(probes.rbegin(), probes.rbegin() + 1, probes.rend());
		probes[0] = ret.raw;
		auto last = probes[1];

		cache.stability_score = 1.0;
		cache.stability_score *= std::max(1.0, std::abs(last.x) - std::abs(ret.raw.x));
		cache.stability_score *= std::max(1.0, std::abs(last.y) - std::abs(ret.raw.y));
		cache.stability_score *= std::max(1.0, std::abs(last.z) - std::abs(ret.raw.z));

		if (cache.scores.size() < 16)
			cache.scores.emplace_back(cache.stability_score);

		if (cache.scores.size() >= 16) {
			std::rotate(cache.scores.rbegin(), cache.scores.rbegin() + 1, cache.scores.rend());
			cache.scores[0] = cache.stability_score;
		}

		cache.stability_score = std::accumulate(cache.scores.begin(), cache.scores.end(), 0.0) / cache.scores.size();
	}

	ret.raw = std::accumulate(probes.begin(), probes.end(), dae::container::axis3d{}) / probes.size();

	constexpr auto degree = 180.0 / std::numbers::pi;
	const auto x = ret.raw.x;
	const auto y = ret.raw.y;
	const auto z = ret.raw.z;

	ret.angles.x = std::numeric_limits<double>::quiet_NaN();
	ret.angles.y = std::atan2(x, z) * degree;
	ret.angles.z = std::atan2(y, std::sqrt(std::pow(x, 2.0) + std::pow(z, 2.0))) * degree;

	cache.prev = ret;

	if constexpr (debug::accelerometer::visible) {
		spdlog::debug("accelerometer:");
		spdlog::debug("  ----------------------");
		spdlog::debug("     raw:  [x]: {:+.0f}", x);
		spdlog::debug("     raw:  [y]: {:+.0f}", y);
		spdlog::debug("     raw:  [z]: {:+.0f}", z);
		spdlog::debug("  ----------------------");
		spdlog::debug("    roll:  [y]: {:+.3f}°", ret.angles.roll());
		spdlog::debug("   pitch:  [z]: {:+.3f}°", ret.angles.pitch());
		spdlog::debug("    time: [Δt]: {:+.3f}s", dt);
		spdlog::debug("  ----------------------");
		spdlog::debug(" stable score: {:+.2f}", cache.stability_score);
	}

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

	const auto wiiremote = fmt::format("wiiremote::{}", id());

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
		while (!queue.empty()) {
			ret.emplace_back(queue.front());
			queue.pop();
		}

		xwii_event event{};
		event.type = XWII_EVENT_NUM;

		if (xwii_iface_dispatch(m_interface, &event, sizeof(event)))
			break;

		auto events = [&]() -> dae::container::events {
			switch (event.type) {
				case XWII_EVENT_ACCEL: return {process::acc(wiimote_acc_state, device::wiimote, event, 0)};
				case XWII_EVENT_BALANCE_BOARD: return {process::press(event)};
				case XWII_EVENT_CLASSIC_CONTROLLER_KEY: return {process_key(device::classic_controller, event)};
				case XWII_EVENT_CLASSIC_CONTROLLER_MOVE: return {process_stick(device::classic_controller, event)};
				case XWII_EVENT_PRO_CONTROLLER_KEY: return {process_key(device::pro_controller, event)};
				case XWII_EVENT_PRO_CONTROLLER_MOVE: return {process_stick(device::pro_controller, event)};
				case XWII_EVENT_GONE: process_gone(); break;
				case XWII_EVENT_IR: return {process::ir(event)};
				case XWII_EVENT_KEY: return {process_key(device::wiimote, event)};
				case XWII_EVENT_MOTION_PLUS: return {process::gyro(wiimote_acc_state, motionp_state, event)};
				case XWII_EVENT_NUNCHUK_KEY:
					return {process_key(device::nunchuk, event)};

				case XWII_EVENT_NUNCHUK_MOVE:
					return {
						process::acc(nunchuk_acc_state, device::nunchuk, event, 1),
						process_stick(device::nunchuk, event)};

				case XWII_EVENT_WATCH:
					process_watch();
					return {};
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
	const auto wiiremote = fmt::format("wiiremote::{}", id());
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

	for (auto &&dev : devices)
		currentExtensionTable[static_cast<std::size_t>(dev)] = is_available(flags, to_xwii_interface(dev));

	for (auto &&dev : devices) {
		const auto idx = static_cast<std::size_t>(dev);
		if (currentExtensionTable[idx] == lastExtensionTable[idx])
			continue;

		const auto available = currentExtensionTable[idx];
		dae::container::status status;
		status.is_connected = available;
		queue.emplace(std::make_pair(dev, std::move(status)));
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
