#pragma once

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <string>

class UInputHelper {
public:
	explicit UInputHelper() = delete;
	virtual ~UInputHelper() = delete;

	static std::string findUinputInterface();

	inline static int set_ev_bit(int fd, int bit) { return ioctl(fd, UI_SET_EVBIT, bit); }
	inline static int set_key_bit(int fd, int bit) { return ioctl(fd, UI_SET_KEYBIT, bit); }
	inline static int set_rel_bit(int fd, int bit) { return ioctl(fd, UI_SET_RELBIT, bit); }
	inline static int set_abs_bit(int fd, int bit) { return ioctl(fd, UI_SET_ABSBIT, bit); }

	inline static void range(struct uinput_user_dev &dev, int axis, int max, int min) {
		dev.absmax[axis] = max;
		dev.absmin[axis] = min;
		dev.absflat[axis] = 0;
		dev.absfuzz[axis] = 0;
	}
};
