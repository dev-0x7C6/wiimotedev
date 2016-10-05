#pragma once

#include "common/enums/device.h"
#include "enums/event.h"

namespace dae {
namespace interface {

class IContainer {
public:
	explicit IContainer() = default;
	virtual ~IContainer() = default;

	virtual common::enums::Device device() const = 0;
	virtual enums::Event event() const = 0;
};
}
}
