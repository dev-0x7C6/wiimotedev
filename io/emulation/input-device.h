#pragma once

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <string>
#include <functional>

#include "include/wiimotedev/wiimotedev"
#include "interfaces/iinput-device.h"

namespace io {
namespace emulation {

class InputDevice : public interface::IInputDevice {
public:
	explicit InputDevice(std::string &&name, u32 id);
	~InputDevice() override;

	bool open() override;
	bool configure() override = 0;
	bool create() override;
	bool destroy() override;
	bool close() override;

	bool isOpen() const override;
	bool isCreated() const override;

protected:
	int set_ev_bit(int bit);
	int set_key_bit(int bit);
	int set_rel_bit(int bit);
	int set_abs_bit(int bit);
	void set_range(int abs, int max, int min);

	bool report(uint16_t type, uint16_t code, int32_t value, bool triggerSync = false);
	bool sync();

protected:
	struct uinput_user_dev m_dev;

private:
	int m_fd;
	bool m_isCreated;
};
}
}
