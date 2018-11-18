#include "infrared-config.h"

bool InfraredConfigContainer::accEnabled() const {
	return m_accEnabled;
}

void InfraredConfigContainer::setAccEnabled(bool accEnabled) {
	m_accEnabled = accEnabled;
}

double InfraredConfigContainer::accMultiX() const {
	return m_accMultiX;
}

void InfraredConfigContainer::setAccMultiX(double accMultiX) {
	m_accMultiX = accMultiX;
}

double InfraredConfigContainer::accMultiY() const {
	return m_accMultiY;
}

void InfraredConfigContainer::setAccMultiY(double accMultiY) {
	m_accMultiY = accMultiY;
}

double InfraredConfigContainer::accPowX() const {
	return m_accPowX;
}

void InfraredConfigContainer::setAccPowX(double accPowX) {
	m_accPowX = accPowX;
}

double InfraredConfigContainer::accPowY() const {
	return m_accPowY;
}

void InfraredConfigContainer::setAccPowY(double accPowY) {
	m_accPowY = accPowY;
}

double InfraredConfigContainer::deadzoneX() const {
	return m_deadzoneX;
}

void InfraredConfigContainer::setDeadzoneX(double deadzoneX) {
	m_deadzoneX = deadzoneX;
}

double InfraredConfigContainer::deadzoneY() const {
	return m_deadzoneY;
}

void InfraredConfigContainer::setDeadzoneY(double deadzoneY) {
	m_deadzoneY = deadzoneY;
}

i32 InfraredConfigContainer::accTimeout() const {
	return m_accTimeout;
}

void InfraredConfigContainer::setAccTimeout(const i32 &accTimeout) {
	m_accTimeout = accTimeout;
}
