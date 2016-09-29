#include "accelerometer-container.h"

using namespace service::container;
using namespace service::interface;
using namespace service::enums;

AccelerometerContainer::AccelerometerContainer(const enums::Device source, const accdata &data)
		: m_source(source)
		, m_data(data) {
}

IContainer::Type AccelerometerContainer::type() const { return Type::Accelerometer; }

Device AccelerometerContainer::deviceType() const { return m_source; }

const accdata &AccelerometerContainer::data() const { return m_data; }
