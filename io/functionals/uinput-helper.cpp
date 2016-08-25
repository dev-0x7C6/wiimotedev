#include "uinput-helper.h"

#include <sys/stat.h>

std::string UInputHelper::findUinputInterface() {
	static auto paths = {
		"/dev/uinput",
		"/dev/input/uinput",
		"/dev/misc/uinput",
	};

	for (const auto &path : paths) {
		struct stat buffer;
		if (stat(path, &buffer) == 0)
			return path;
	}

	return {};
}
