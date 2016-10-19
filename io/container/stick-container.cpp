#include "stick-container.h"

using namespace io::container;

void AxisPair::center() {
	for (const auto &axis : {AxisType::X, AxisType::Y})
		pair[axis].value = (pair[axis].raw_min + pair[axis].raw_max) / 2;
}

void AxisPair::setValue(int32_t x, int32_t y) {
	if (pair[X].min >= 0 || pair[Y].min >= 0) {
		x = (m_invertXAxis) ? x ^ pair[X].raw_max : x;
		y = (m_invertYAxis) ? y ^ pair[Y].raw_max : y;
	} else {
		x = (m_invertXAxis) ? x * -1 : x;
		y = (m_invertYAxis) ? y * -1 : y;
	}
	pair[X].value = std::max(pair[X].min, std::min(x, pair[X].max));
	pair[Y].value = std::max(pair[Y].min, std::min(y, pair[Y].max));
}
