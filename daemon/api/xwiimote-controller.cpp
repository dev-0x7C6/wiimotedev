#include "xwiimote-controller.h"

#include <poll.h>
#include <xwiimote.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "containers/infrared-container.h"
#include "containers/accelerometer-container.h"

using namespace daemon::api;
using namespace daemon::container;

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

std::unique_ptr<daemon::interface::IContainer> XWiimoteController::process() {
	struct xwii_event event;
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

	if (ret)
		return nullptr;

	switch (event.type) {
		case XWII_EVENT_IR: return std::make_unique<InfraredContainer>(event);
		case XWII_EVENT_ACCEL: return std::make_unique<AccelerometerContainer>(AccelerometerContainer::Source::Wiimote, event);
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
