#pragma once

#include "enums/device-type.h"

namespace service {
namespace interface {

class IContainer {
public:
	enum class Type {
		Empty,
		Infrared,
		Button,
		Stick,
		Accelerometer,
		Gyroscope,
		Pressure,
		Status,
	};

	explicit IContainer() = default;
	virtual ~IContainer() = default;

	virtual Type type() const = 0;
	virtual enums::Device deviceType() const = 0;
};
}
}
