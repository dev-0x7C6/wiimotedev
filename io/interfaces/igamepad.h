#pragma once

#include "common/enums/device.h"
#include "container/stick-container.h"
#include "emulation/input-device.h"

#include <QJsonObject>

namespace io {
namespace interface {

class IGamepad : public emulation::InputDevice {
public:
	explicit IGamepad(const std::string &name, const uint32_t id, container::ButtonMap &&buttons, container::AxisMap &&axises);
	virtual ~IGamepad() = default;

	static common::enums::Device fromString(const std::string &type);

	virtual common::enums::Device type() const = 0;
	virtual bool configure() override;

	virtual bool input(const int tl, const int tr, const int bl, const int br);
	virtual bool input(const uint64_t buttons);
	virtual bool input(const container::Stick stick, const int32_t x, const int32_t y);

	virtual int32_t axisMax(const int32_t axis);
	virtual int32_t axisMin(const int32_t axis);

	void setConfiguration(const QJsonObject &json);
	const QJsonObject &configuration();

protected:
	QJsonObject m_configuration;
	container::ButtonMap m_buttons;
	container::AxisMap m_axises;
};
}
}
