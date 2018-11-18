#include "input-device.h"

#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "functionals/uinput-helper.h"

using namespace io::emulation;

constexpr auto UINPUT_PRINT_VALID_EVENTS = false;

InputDevice::InputDevice(std::string &&name, const u32 id)
		: IInputDevice(std::move(name), id)
		, m_fd(-1)
		, m_isCreated(false)

{
}

InputDevice::~InputDevice() {
	if (isCreated())
		destroy();
	if (isOpen())
		close();
}

bool InputDevice::open() {
	auto path = UInputHelper::findUinputInterface();
	std::cout << std::endl;

	if (path.empty()) {
		std::cerr << "fail: unable to find uinput interface!" << std::endl;
		return false;
	}

	std::cout << "ok: uinput interface found - " << path << "." << std::endl;

	m_fd = ::open(path.c_str(), O_WRONLY | O_NDELAY);

	if (m_fd == -1) {
		std::cerr << "fail: unable to open uinput interface!" << std::endl;
		return false;
	}

	std::cout << "ok: uinput interface opened." << std::endl;

	memset(&m_dev, 0, sizeof(m_dev));
	memcpy(m_dev.name, m_name.c_str(), std::min(m_name.size(), sizeof(m_dev.name)));
	m_dev.id.product = 1;
	m_dev.id.version = 1;
	m_dev.id.vendor = 1;
	m_dev.id.bustype = BUS_USB;

	std::cout << "ok: preparing device header." << std::endl;
	std::cout << "  name     : " << m_dev.name << std::endl;
	std::cout << "  product  : " << m_dev.id.product << std::endl;
	std::cout << "  version  : " << m_dev.id.version << std::endl;
	std::cout << "  vendor   : " << m_dev.id.vendor << std::endl;
	std::cout << "  bustype  : " << m_dev.id.bustype << std::endl;
	std::cout << std::endl;

	return isOpen();
}

bool InputDevice::create() {
	auto result = write(m_fd, &m_dev, sizeof(m_dev));

	if (result != sizeof(m_dev)) {
		std::cerr << "fail: unable to write device header." << std::endl;
		m_isCreated = false;
		return false;
	}

	std::cout << "ok: successfully write device header." << std::endl;

	if (ioctl(m_fd, UI_DEV_CREATE)) {
		std::cerr << "fail: unable create virtual device." << std::endl;
		m_isCreated = false;
		return false;
	}

	std::cout << "ok: virtual device created :-)" << std::endl;
	m_isCreated = true;
	return m_isCreated;
}

bool InputDevice::destroy() {
	auto status = ioctl(m_fd, UI_DEV_DESTROY) == 0;
	if (m_isCreated && !status)
		return false;

	m_isCreated = status;
	return true;
}

bool InputDevice::close() { return ::close(m_fd) == 0; }

bool InputDevice::isOpen() const { return m_fd != -1; }
bool InputDevice::isCreated() const { return m_isCreated; }

int InputDevice::set_ev_bit(int bit) { return UInputHelper::set_ev_bit(m_fd, bit); }
int InputDevice::set_key_bit(int bit) { return UInputHelper::set_key_bit(m_fd, bit); }
int InputDevice::set_rel_bit(int bit) { return UInputHelper::set_rel_bit(m_fd, bit); }
int InputDevice::set_abs_bit(int bit) { return UInputHelper::set_abs_bit(m_fd, bit); }
void InputDevice::set_range(int abs, int max, int min) { UInputHelper::range(m_dev, abs, max, min); }

bool InputDevice::report(u16 type, u16 code, i32 value, bool triggerSync) {
	if (!isCreated())
		return false;

	struct input_event event;
	memset(&event.time, 0, sizeof(event.time));
	event.code = code;
	event.type = type;
	event.value = value;
	auto result = write(m_fd, &event, sizeof(event));

	if (result != sizeof(event)) {
		std::cerr << "fail: unable to write event." << std::endl;
		std::cerr << "  code   : " << code << std::endl;
		std::cerr << "  type   : " << type << std::endl;
		std::cerr << "  value  : " << value << std::endl;
		std::cerr << std::endl;
		m_isCreated = false;
		return false;
	}

	if (UINPUT_PRINT_VALID_EVENTS) {
		std::cout << "ok: event written." << std::endl;
		std::cout << "  code   : " << code << std::endl;
		std::cout << "  type   : " << type << std::endl;
		std::cout << "  value  : " << value << std::endl;
		std::cout << std::endl;
	}

	if (triggerSync)
		sync();

	return true;
}

bool InputDevice::sync() { return report(EV_SYN, SYN_REPORT, false); }
