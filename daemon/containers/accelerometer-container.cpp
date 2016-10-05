#include "accelerometer-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::container::structs;
using namespace dae::enums;
using namespace dae::interface;

AccelerometerContainer::AccelerometerContainer(const common::enums::Device source, const accdata &data)
		: m_source(source)
		, m_data(data) {
}

Event AccelerometerContainer::event() const { return Event::Accelerometer; }

Device AccelerometerContainer::device() const { return m_source; }

const accdata &AccelerometerContainer::data() const { return m_data; }
