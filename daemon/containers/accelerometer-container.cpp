#include "accelerometer-container.h"

using namespace daemon::container;
using namespace daemon::interface;

AccelerometerContainer::AccelerometerContainer(const Source source, const accdata &data)
		: m_source(source)
		, m_data(data) {
}

IContainer::Type AccelerometerContainer::type() const { return Type::Accelerometer; }

IContainer::Source AccelerometerContainer::source() const { return m_source; }

const accdata &AccelerometerContainer::data() const { return m_data; }
