#pragma once

#include "enums/device-type.h"
#include "enums/event.h"

namespace service {
namespace interface {

class IContainer {
public:
	explicit IContainer() = default;
	virtual ~IContainer() = default;

	virtual enums::Device device() const = 0;
	virtual enums::Event event() const = 0;
};
}
}
