#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/consts.h"

namespace service {
namespace container {

class AccelerometerContainer final : public interface::IContainer {
public:
	explicit AccelerometerContainer(const enums::Device device, const accdata &data);
	virtual ~AccelerometerContainer() = default;

	virtual enums::Device device() const override;
	virtual enums::Event event() const override;

	const accdata &data() const;

private:
	const enums::Device m_source;
	accdata m_data;
};
}
}
