#include "virtual-cursor-processor.h"

using namespace dae::functional;

VirtualCursorProcessor::VirtualCursorProcessor() {

	for (std::size_t i = 0; i < 2; ++i) {
		m_ctable[i][0] = 0x00;
		m_ctable[i][1] = 0x00;
		m_ltable[i][0] = 0x00;
		m_ltable[i][1] = 0x00;
	}

	m_calibrationPoint[0] = m_calibrationPoint[1] = 0x00;
}

double VirtualCursorProcessor::angleDiff(double a, double b) {
	double diff = b - a;

	while (diff > 180)
		diff -= 360;

	while (diff <= -180)
		diff += 360;

	return diff;
}

double VirtualCursorProcessor::_distance(i16 p[], i16 q[]) {
	double dx = p[0] - q[0];
	double dy = p[1] - q[1];
	dx = sqrt(dx * dx + dy * dy);

	if (dx < 0)
		dx *= -1;

	return dx;
}

bool VirtualCursorProcessor::calculate(QList<QPair<int, int>> &points, double roll) {
	switch (points.count()) {
		case 4:
		case 3:
		case 2:
			m_ltable[0][0] = m_ctable[0][0] = points.at(0).first;
			m_ltable[1][0] = m_ctable[1][0] = points.at(1).first;
			m_ltable[0][1] = m_ctable[0][1] = points.at(0).second;
			m_ltable[1][1] = m_ctable[1][1] = points.at(1).second;
			m_calibrationPoint[0] = m_calibrationPoint[1] = 0x00;
			m_visible = true;
			break;
		case 1:
			if (m_calibrationPoint[0] == 0x00) m_calibrationPoint[0] = points.at(0).first;
			if (m_calibrationPoint[1] == 0x00) m_calibrationPoint[1] = points.at(0).second;
			m_visible = true;
			break;
		case 0:
			return false;
	}

	m_angle[0] = atan2(m_ctable[1][1] - m_ctable[0][1], m_ctable[1][0] - m_ctable[0][0]) - M_PI;
	m_angle[1] = atan2(m_ctable[0][1] - m_ctable[1][1], m_ctable[0][0] - m_ctable[1][0]) - M_PI;
	m_index = (abs(angleDiff(m_angle[0] * 180 / M_PI, roll)) < 90) ? 0 : 1;

	double cosp = cos(-m_angle[m_index]);
	double sinp = sin(-m_angle[m_index]);
	double sx = (m_ctable[0][0] + m_ctable[1][0]) / 2.0;
	double sy = (m_ctable[0][1] + m_ctable[1][1]) / 2.0;

	if (points.count() == 1) {
		sx -= (m_calibrationPoint[0] - points.at(0).first);
		sy -= (m_calibrationPoint[1] - points.at(0).second);
	}

	m_distance = _distance(m_ctable[0], m_ctable[1]);
	m_x = (512.0 - ((1024.0 - (sx * cosp - sy * sinp + 512.0 * (1 - cosp) + 384.0 * sinp))));
	m_y = (384.0 - ((sx * sinp + sy * cosp - 512.0 * sinp + 384.0 * (1 - cosp))));
	return true;
}
