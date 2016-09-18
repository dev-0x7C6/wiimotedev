#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "containers/accelerometer-container.h"
#include "containers/infrared-container.h"
#include "containers/gyroscope-container.h"
#include "containers/pressure-container.h"
#include "interfaces/icontainer.h"

using namespace service::api;
using namespace service::container;
using namespace service::interface;

XWiimoteController::XWiimoteController(const std::string &interfaceFilePath)
		: m_interfaceFilePath(interfaceFilePath) {
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

std::unique_ptr<service::interface::IContainer> XWiimoteController::process() {
	struct xwii_event event;
	memset(&event, 0, sizeof(event));
	static pollfd fds[2];
	memset(fds, 0, sizeof(fds));
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[1].fd = m_fd;
	fds[1].events = POLLIN;
	auto ret = poll(fds, 2, -1);

	if (ret < 0) {
		if (errno != EINTR) {
			ret = -errno;
			std::cerr << "Error: Cannot poll fds:" << ret << std::endl;
		}
	}

	ret = xwii_iface_dispatch(m_interface, &event, sizeof(event));

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

	if (ret)
		return nullptr;

	switch (event.type) {
		case XWII_EVENT_IR: return process_ir(event);
		case XWII_EVENT_ACCEL: return process_acc(event, IContainer::Source::Wiimote);
		case XWII_EVENT_MOTION_PLUS: return process_gyro(event);
		case XWII_EVENT_NUNCHUK_MOVE: return process_acc(event, IContainer::Source::Nunchuk);
		case XWII_EVENT_BALANCE_BOARD:
			return process_press(event);
		//case XWII_EVENT_KEY: return process_button(event, IContainer::Source::Wiimote);
		default:
			break;
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
	std::cout << flags << std::endl;
	auto ret = xwii_iface_open(m_interface, flags);

	if (ret) {
		std::cerr << "fail: unable to open " << m_interfaceFilePath << " interface." << std::endl;
		std::cerr << xwii_iface_opened(m_interface) << std::endl;
		xwii_iface_unref(m_interface);
		m_interface = nullptr;
		return;
	}
}
