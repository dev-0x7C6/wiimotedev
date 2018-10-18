#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote.h>

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "include/wiimotedev/wiimotedev"
#include "containers/accelerometer-container.h"
#include "containers/infrared-container.h"
#include "containers/gyroscope-container.h"
#include "containers/pressure-container.h"
#include "containers/button-container.h"
#include "containers/status-container.h"
#include "containers/stick-container.h"
#include "interfaces/icontainer.h"

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

XWiimoteController::XWiimoteController(IIdManager &manager, const std::string &interfaceFilePath)
		: IWiimote(manager)
		, m_interfaceFilePath(interfaceFilePath) {
	m_buttons.fill(0);
	auto ret = xwii_iface_new(&m_interface, m_interfaceFilePath.c_str());

	if (ret) {
		std::cerr << "fail: unable to create " << m_interfaceFilePath << " interface." << std::endl;
		return;
	}

	m_fd = xwii_iface_get_fd(m_interface);

	if (m_fd < 0) {
		std::cerr << "fail: cannot get decriptor for interface." << std::endl;
		return;
	}

	ret = xwii_iface_watch(m_interface, true);

	if (ret) {
		std::cerr << "fail: unable to attach watcher on interface." << std::endl;
		return;
	}

	reconfigure();

	m_connected = true;

	setId(m_idManager.take(type()));
}

XWiimoteController::~XWiimoteController() {
	xwii_iface_close(m_interface, XWII_IFACE_ALL | XWII_IFACE_WRITABLE);
	xwii_iface_unref(m_interface);
	m_idManager.debt(type(), id());
}

Device XWiimoteController::type() const {
	const auto flags = xwii_iface_available(m_interface);

	if (flags & XWII_IFACE_BALANCE_BOARD)
		return Device::BalanceBoard;

	if (flags & XWII_IFACE_PRO_CONTROLLER)
		return Device::ProController;

	return Device::Wiimote;
}

bool XWiimoteController::isValid() const { return m_connected; }

std::unique_ptr<dae::interface::IContainer> XWiimoteController::process() {
	auto process_ir = [](const xwii_event &event) {
		IrPoints ir;
		for (std::size_t i = 0; i < ir.size(); ++i) {
			ir[i].x = event.v.abs[i].x;
			ir[i].y = event.v.abs[i].y;
			ir[i].size = xwii_event_ir_is_valid(&event.v.abs[i]) ? 1 : -1;
		}

		return std::make_unique<InfraredContainer>(ir);
	};

	auto process_acc = [](const Device source, const xwii_event &event, const int axis) {
		struct accdata data;
		data.x = event.v.abs[axis].x;
		data.y = event.v.abs[axis].y;
		data.z = event.v.abs[axis].z;
		data.roll = 0;
		data.pitch = 0;
		return std::make_unique<AccelerometerContainer>(source, data);
	};

	auto process_press = [](const xwii_event &event) {
		struct pressdata data;
		data.tl = event.v.abs[2].x;
		data.bl = event.v.abs[3].x;
		data.br = event.v.abs[1].x;
		data.tr = event.v.abs[0].x;
		return std::make_unique<PressureContainer>(data);
	};

	auto process_key = [this](const Device source, const xwii_event &event) {
		auto mask = m_buttons.at(static_cast<std::size_t>(source));
		auto code = 1ull << event.v.key.code;
		static_assert(sizeof(code) == 8);

		if (event.v.key.state)
			mask |= code;
		else
			mask &= ~code;

		m_buttons.at(static_cast<std::size_t>(source)) = mask;
		return std::make_unique<ButtonContainer>(source, mask);
	};

	auto process_gone = [this]() {
		m_connected = false;
		return nullptr;
		//return std::make_unique<StatusContainer>(Device::Wiimote, StatusContainer::State::Disconnected);
	};

	auto process_stick = [](Device device, const xwii_event &event) {
		auto calculate = [](int32_t &x, int32_t &y, int32_t step) {
			constexpr int32_t min = WIIMOTEDEV_STICK_MIN;
			constexpr int32_t max = WIIMOTEDEV_STICK_MAX;
			x = std::max(-step, std::min(step, x)) * (max / step);
			y = std::max(-step, std::min(step, y)) * (max / step);
			x = std::max(min, std::min(max, x));
			y = std::max(min, std::min(max, y));
		};

		int32_t lx = event.v.abs[0].x;
		int32_t ly = event.v.abs[0].y;
		int32_t rx = event.v.abs[1].x;
		int32_t ry = event.v.abs[1].y;

		if (device == Device::Nunchuk) {
			calculate(lx, ly, 100);
			return std::make_unique<StickContainer>(device, lx, ly, 0, 0);
		}

		if (device == Device::Classic) {
			calculate(lx, ly, 24);
			calculate(rx, ry, 24);
			return std::make_unique<StickContainer>(device, lx, ly, rx, ry);
		}

		if (device == Device::ProController) {
			calculate(lx, ly, 1280);
			calculate(rx, ry, 1280);
			ly *= -1;
			ry *= -1;
			return std::make_unique<StickContainer>(device, lx, ly, rx, ry);
		}

		return std::make_unique<StickContainer>(device,
			event.v.abs[0].x, event.v.abs[0].y, event.v.abs[1].x, event.v.abs[1].y);
	};

	auto process_watch = [this]() {
		reconfigure();
		return nullptr;
	};

	struct xwii_event event;
	event.type = XWII_EVENT_NUM;
	auto ret = xwii_iface_dispatch(m_interface, &event, sizeof(event));

	if (ret) {
		if (!m_messages.empty()) {
			auto result = std::move(m_messages.front());
			m_messages.pop();
			return result;
		}
		return nullptr;
	}

	if (!m_messages.empty()) {
		auto result = std::move(m_messages.front());
		m_messages.pop();
		return result;
	}

	switch (event.type) {
		case XWII_EVENT_ACCEL: return process_acc(Device::Wiimote, event, 0);
		case XWII_EVENT_BALANCE_BOARD: return process_press(event);
		case XWII_EVENT_CLASSIC_CONTROLLER_KEY: return process_key(Device::Classic, event);
		case XWII_EVENT_CLASSIC_CONTROLLER_MOVE: return process_stick(Device::Classic, event);
		case XWII_EVENT_PRO_CONTROLLER_KEY: return process_key(Device::ProController, event);
		case XWII_EVENT_PRO_CONTROLLER_MOVE: return process_stick(Device::ProController, event);
		case XWII_EVENT_GONE: return process_gone();
		case XWII_EVENT_IR: return process_ir(event);
		case XWII_EVENT_KEY: return process_key(Device::Wiimote, event);
		case XWII_EVENT_MOTION_PLUS: return std::make_unique<GyroscopeContainer>(event.v.abs[0].x, event.v.abs[0].y, event.v.abs[0].z);
		case XWII_EVENT_NUNCHUK_KEY: return process_key(Device::Nunchuk, event);
		case XWII_EVENT_NUNCHUK_MOVE:
			m_messages.emplace(process_acc(Device::Nunchuk, event, 1));
			return process_stick(Device::Nunchuk, event);
		case XWII_EVENT_WATCH: return process_watch();
	}

	return nullptr;
}

bool XWiimoteController::isRumbleSupported() {
	return true;
}

bool XWiimoteController::isLedSupported() {
	return true;
}

bool XWiimoteController::isInfraredSupported() {
	const auto flags = xwii_iface_available(m_interface);
	return (flags & XWII_IFACE_IR) == XWII_IFACE_IR;
}

uint8_t XWiimoteController::batteryStatus() {
	uint8_t capacity = 0;
	xwii_iface_get_battery(m_interface, &capacity);
	return capacity;
}

bool XWiimoteController::ledStatus(const uint32_t id) {
	bool state = false;
	xwii_iface_get_led(m_interface, id, &state);
	return state;
}

bool XWiimoteController::rumbleStatus() {
	return m_rumbleStatus;
}

bool XWiimoteController::setLedStatus(const uint32_t id, const bool status) {
	return xwii_iface_set_led(m_interface, id, status) == 0;
}

bool XWiimoteController::setRumbleStatus(const bool rumble) {
	const auto isValid = xwii_iface_rumble(m_interface, rumble) == 0;
	if (isValid)
		m_rumbleStatus = rumble;
	return isValid;
}

bool XWiimoteController::hasClassicExtension() {
	const auto flags = xwii_iface_available(m_interface);
	return (flags & XWII_IFACE_CLASSIC_CONTROLLER) == XWII_IFACE_CLASSIC_CONTROLLER;
}

bool XWiimoteController::hasMotionPlusExtension() {
	const auto flags = xwii_iface_available(m_interface);
	return (flags & XWII_IFACE_MOTION_PLUS) == XWII_IFACE_MOTION_PLUS;
}

bool XWiimoteController::hasNunchukExtension() {
	const auto flags = xwii_iface_available(m_interface);
	return (flags & XWII_IFACE_NUNCHUK) == XWII_IFACE_NUNCHUK;
}

std::string XWiimoteController::interfaceFilePath() const {
	return m_interfaceFilePath;
}

void XWiimoteController::reconfigure() {
	auto flags = xwii_iface_available(m_interface) | XWII_IFACE_WRITABLE;
	auto ret = xwii_iface_open(m_interface, flags);

	if ((flags & XWII_IFACE_CORE) && !m_wiimoteConnected) {
		m_wiimoteConnected = true;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Wiimote, StatusContainer::State::Connected));
	}

	if ((flags & XWII_IFACE_CLASSIC_CONTROLLER) && !m_classicControllerConnected) {
		m_classicControllerConnected = true;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Classic, StatusContainer::State::Connected));
	}

	if ((flags & XWII_IFACE_NUNCHUK) && !m_nunchukConnected) {
		m_nunchukConnected = true;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Nunchuk, StatusContainer::State::Connected));
	}

	if ((flags & XWII_IFACE_MOTION_PLUS) && !m_motionPlusConnected) {
		m_motionPlusConnected = true;
		//m_messages.emplace(std::make_unique<StatusContainer>(Device::, StatusContainer::State::Connected));
	}

	if ((flags & XWII_IFACE_BALANCE_BOARD) && !m_balanceBoardConnected) {
		m_balanceBoardConnected = true;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::BalanceBoard, StatusContainer::State::Connected));
	}

	if ((flags & XWII_IFACE_PRO_CONTROLLER) && !m_proControllerConnected) {
		m_proControllerConnected = true;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::ProController, StatusContainer::State::Connected));
	}

	if (!(flags & XWII_IFACE_CORE) && m_wiimoteConnected) {
		m_wiimoteConnected = false;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Wiimote, StatusContainer::State::Disconnected));
	}

	if (!(flags & XWII_IFACE_CLASSIC_CONTROLLER) && m_classicControllerConnected) {
		m_classicControllerConnected = false;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Classic, StatusContainer::State::Disconnected));
	}

	if (!(flags & XWII_IFACE_NUNCHUK) && m_nunchukConnected) {
		m_nunchukConnected = false;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::Nunchuk, StatusContainer::State::Disconnected));
	}

	if (!(flags & XWII_IFACE_MOTION_PLUS) && m_motionPlusConnected) {
		m_motionPlusConnected = false;
		//m_messages.emplace(std::make_unique<StatusContainer>(Device::Wiimote, StatusContainer::State::Disconnected));
	}

	if (!(flags & XWII_IFACE_BALANCE_BOARD) && m_balanceBoardConnected) {
		m_balanceBoardConnected = false;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::BalanceBoard, StatusContainer::State::Disconnected));
	}

	if (!(flags & XWII_IFACE_PRO_CONTROLLER) && m_proControllerConnected) {
		m_proControllerConnected = false;
		m_messages.emplace(std::make_unique<StatusContainer>(Device::ProController, StatusContainer::State::Disconnected));
	}

	//	constexpr auto space = 25;
	//	std::cout << "report mode:" << std::endl;
	//	std::cout << std::boolalpha << std::left;
	//	std::cout << std::setw(space) << "  core"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_CORE) << std::endl;
	//	std::cout << std::setw(space) << "  accelerometer"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_ACCEL) << std::endl;
	//	std::cout << std::setw(space) << "  gyroscope"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_MOTION_PLUS) << std::endl;
	//	std::cout << std::setw(space) << "  nunchuk"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_NUNCHUK) << std::endl;
	//	std::cout << std::setw(space) << "  classic"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_CLASSIC_CONTROLLER) << std::endl;
	//	std::cout << std::setw(space) << "  balance board"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_BALANCE_BOARD) << std::endl;
	//	std::cout << std::setw(space) << "  pro controller"
	//			  << ": " << static_cast<bool>(flags & XWII_IFACE_PRO_CONTROLLER) << std::endl;
	//	std::cout << std::noboolalpha;

	if (ret) {
		std::cerr << "fail: unable to open " << m_interfaceFilePath << " interface." << std::endl;
		xwii_iface_close(m_interface, 0);
		xwii_iface_unref(m_interface);
		m_interface = nullptr;
		m_connected = false;
		return;
	}
}
