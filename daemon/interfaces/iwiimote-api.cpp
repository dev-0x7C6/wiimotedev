#include "iwiimote-api.h"

using namespace dae::interface;

IWiimote::IWiimote(IIdManager &manager)
		: m_idManager(manager) {
}

uint32_t IWiimote::id() const { return m_id; }
void IWiimote::setId(const uint32_t &id) { m_id = id; }
