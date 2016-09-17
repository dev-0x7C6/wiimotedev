#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/consts.h"

namespace daemon {
namespace container {

class PressureContainer final : public interface::IContainer {
public:
	explicit PressureContainer(const pressdata &data);
	virtual ~PressureContainer() = default;

	virtual Source source() const override;
	virtual Type type() const override;

	const pressdata &data() const;

private:
	const pressdata m_data;
};
}
}
