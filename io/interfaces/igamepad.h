#pragma once

#include "common/enums/device.h"
#include "container/stick-container.h"
#include "emulation/input-device.h"

#include <QJsonObject>

namespace io {
namespace interface {

class IGamepad : public emulation::InputDevice {
public:
	explicit IGamepad(std::string &&name, u32 id, container::ButtonMap &&buttons, container::AxisMap &&axises);

	static common::enums::device fromString(const std::string &type);

	virtual common::enums::device type() const = 0;
	bool configure() override;

	bool input(int tl, int tr, int bl, int br);
	bool input(u64 buttons);
	bool input(container::Stick stick, i32 x, i32 y);

	i32 axisMax(i32 axis);
	i32 axisMin(i32 axis);

	void setConfiguration(const QJsonObject &json);
	const QJsonObject &configuration();

protected:
	QJsonObject m_configuration;
	container::ButtonMap m_buttons;
	container::AxisMap m_axises;
};
}
}
