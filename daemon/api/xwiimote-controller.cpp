#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote.h>

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "containers/accelerometer-container.h"
#include "containers/infrared-container.h"
#include "containers/gyroscope-container.h"
#include "containers/pressure-container.h"
#include "containers/button-container.h"
#include "interfaces/icontainer.h"

using namespace service::api;
using namespace service::container;
using namespace service::interface;

XWiimoteController::XWiimoteController(const std::string &interfaceFilePath)
		: m_interfaceFilePath(interfaceFilePath) {
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
}

XWiimoteController::~XWiimoteController() {
	xwii_iface_close(m_interface, XWII_IFACE_ALL | XWII_IFACE_WRITABLE);
	xwii_iface_unref(m_interface);
}

IWiimote::Type XWiimoteController::type() const {
	const auto flags = xwii_iface_available(m_interface);

	if (flags & XWII_IFACE_BALANCE_BOARD)
		return Type::BalanceBoard;

	if (flags & XWII_IFACE_PRO_CONTROLLER)
		return Type::ProController;

	return Type::Wiimote;
}

bool XWiimoteController::isValid() const { return m_connected; }

std::unique_ptr<service::interface::IContainer> XWiimoteController::process() {
	auto process_ir = [](const xwii_event &event) {
		IrPoints ir;
		for (std::size_t i = 0; i < ir.size(); ++i) {
			ir[i].x = event.v.abs[i].x;
			ir[i].y = event.v.abs[i].y;
			ir[i].size = xwii_event_ir_is_valid(&event.v.abs[i]) ? 1 : -1;
		}

		return std::make_unique<InfraredContainer>(ir);
	};

	auto process_acc = [](const xwii_event &event, const IContainer::Source source) {
		struct accdata data;
		data.x = event.v.abs[0].x;
		data.y = event.v.abs[0].y;
		data.z = event.v.abs[0].z;
		data.roll = 0;
		data.pitch = 0;
		return std::make_unique<AccelerometerContainer>(source, data);
	};

	auto process_gyro = [](const xwii_event &event) {
		struct gyrodata data;
		data.x = event.v.abs[0].x;
		data.y = event.v.abs[0].y;
		data.z = event.v.abs[0].z;
		data.lowX = event.v.abs[1].x;
		data.lowY = event.v.abs[1].y;
		data.lowZ = event.v.abs[1].z;
		return std::make_unique<GyroscopeContainer>(data);
	};

	auto process_press = [](const xwii_event &event) {
		struct pressdata data;
		data.tl = event.v.abs[2].x;
		data.bl = event.v.abs[3].x;
		data.br = event.v.abs[1].x;
		data.tr = event.v.abs[0].x;
		return std::make_unique<PressureContainer>(data);
	};

	auto process_key = [this](const IContainer::Source source, const xwii_event &event) {
		auto mask = m_buttons.at(static_cast<std::size_t>(source));

		switch (event.v.key.code) {
			case XWII_KEY_LEFT:
			case XWII_KEY_RIGHT:
			case XWII_KEY_UP:
			case XWII_KEY_DOWN:
			case XWII_KEY_A:
			case XWII_KEY_B:
			case XWII_KEY_PLUS:
			case XWII_KEY_MINUS:
			case XWII_KEY_HOME:
			case XWII_KEY_ONE:
			case XWII_KEY_TWO:
			case XWII_KEY_X:
			case XWII_KEY_Y:
			case XWII_KEY_TL:
			case XWII_KEY_TR:
			case XWII_KEY_ZL:
			case XWII_KEY_ZR:
			case XWII_KEY_THUMBL:
			case XWII_KEY_THUMBR:
			case XWII_KEY_C:
			case XWII_KEY_Z:
				break;
		}

		const auto toggled = event.v.key.state == 0 || event.v.key.state == 2;

		return std::make_unique<ButtonContainer>(source, event.v.key.code);
	};

	auto process_gone = [this]() {
		m_connected = false;
		return nullptr;
	};

	auto process_watch = [this]() {
		std::cout << "hotplug" << std::endl;
		reconfigure();
		return nullptr;
	};

	struct xwii_event event;
	event.type = XWII_EVENT_NUM;
	auto ret = xwii_iface_dispatch(m_interface, &event, sizeof(event));

	if (ret || !m_connected) {
		return nullptr;
	}

	switch (event.type) {
		case XWII_EVENT_ACCEL: return process_acc(event, IContainer::Source::Wiimote);
		case XWII_EVENT_BALANCE_BOARD: return process_press(event);
		case XWII_EVENT_CLASSIC_CONTROLLER_KEY: return process_key(IContainer::Source::Classic, event);
		case XWII_EVENT_PRO_CONTROLLER_KEY: return process_key(IContainer::Source::ProController, event);
		case XWII_EVENT_GONE: return process_gone();
		case XWII_EVENT_IR: return process_ir(event);
		case XWII_EVENT_KEY: return process_key(IContainer::Source::Wiimote, event);
		case XWII_EVENT_MOTION_PLUS: return process_gyro(event);
		case XWII_EVENT_NUNCHUK_KEY: return process_key(IContainer::Source::Nunchuk, event);
		case XWII_EVENT_NUNCHUK_MOVE: return process_acc(event, IContainer::Source::Nunchuk);
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
	return true;
}

bool XWiimoteController::setLedStatus(const uint32_t id, const bool status) {
	return xwii_iface_set_led(m_interface, id, status) == 0;
}

bool XWiimoteController::setRumbleStatus(const bool rumble) {
	return xwii_iface_rumble(m_interface, rumble) == 0;
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

	constexpr auto space = 25;
	std::cout << "report mode:" << std::endl;
	std::cout << std::boolalpha << std::left;
	std::cout << std::setw(space) << "  core"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_CORE) << std::endl;
	std::cout << std::setw(space) << "  accelerometer"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_ACCEL) << std::endl;
	std::cout << std::setw(space) << "  gyroscope"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_MOTION_PLUS) << std::endl;
	std::cout << std::setw(space) << "  nunchuk"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_NUNCHUK) << std::endl;
	std::cout << std::setw(space) << "  classic"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_CLASSIC_CONTROLLER) << std::endl;
	std::cout << std::setw(space) << "  pro controller"
			  << ": " << static_cast<bool>(flags & XWII_IFACE_PRO_CONTROLLER) << std::endl;
	std::cout << std::noboolalpha;

	if (ret) {
		std::cerr << "fail: unable to open " << m_interfaceFilePath << " interface." << std::endl;
		xwii_iface_close(m_interface, 0);
		xwii_iface_unref(m_interface);
		m_interface = nullptr;
		m_connected = false;
		return;
	}
}
