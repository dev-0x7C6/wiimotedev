#include "accelerometer-container.h"
#include <xwiimote.h>

using namespace daemon::container;
using namespace daemon::interface;

AccelerometerContainer::AccelerometerContainer(const Source source, const accdata &data)
		: m_source(source)
		, m_data(data) {
}

AccelerometerContainer::AccelerometerContainer(const Source source, const xwii_event &event)
		: m_source(source)
		, m_data()

{
	m_data.x = event.v.abs[0].x;
	m_data.y = event.v.abs[0].y;
	m_data.z = event.v.abs[0].z;
	m_data.roll = 0;
	m_data.pitch = 0;
}

IContainer::Type AccelerometerContainer::type() const { return Type::Accelerometer; }

IContainer::Source AccelerometerContainer::source() const { return m_source; }

const accdata &AccelerometerContainer::data() const { return m_data; }
