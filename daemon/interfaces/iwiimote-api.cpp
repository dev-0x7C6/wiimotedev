#include "iwiimote-api.h"

using namespace dae::interface;

IWiimote::IWiimote(IIdManager &manager)
		: m_idManager(manager) {
}

u32 IWiimote::id() const { return m_id; }
void IWiimote::setId(const u32 &id) { m_id = id; }
