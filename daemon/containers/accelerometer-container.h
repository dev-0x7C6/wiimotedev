#pragma once

#include "interfaces/icontainer.h"
#include "linux/usr/include/wiimotedev/consts.h"

struct xwii_event;

namespace daemon {
namespace container {

class AccelerometerContainer final : public interface::IContainer {
public:
	explicit AccelerometerContainer(const Source source, const accdata &data);
	explicit AccelerometerContainer(const Source source, const xwii_event &event);
	virtual ~AccelerometerContainer() = default;

	virtual Type type() const override;
	virtual Source source() const override;

	const accdata &data() const;

private:
	const Source m_source;
	accdata m_data;
};
}
}
