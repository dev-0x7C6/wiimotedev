#include "accelerometer-container.h"

using namespace service::container;
using namespace service::container::structs;
using namespace service::enums;
using namespace service::interface;

AccelerometerContainer::AccelerometerContainer(const enums::Device source, const accdata &data)
		: m_source(source)
		, m_data(data) {
}

Event AccelerometerContainer::event() const { return Event::Accelerometer; }

Device AccelerometerContainer::device() const { return m_source; }

const accdata &AccelerometerContainer::data() const { return m_data; }
