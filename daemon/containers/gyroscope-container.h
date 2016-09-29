#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/consts.h"

namespace service {
namespace container {

class GyroscopeContainer final : public interface::IContainer {
public:
	explicit GyroscopeContainer(const struct gyrodata &data);
	virtual ~GyroscopeContainer() = default;

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	gyrodata data() const;

private:
	const gyrodata m_data;
};
}
}
