#include "iwiimote-api.h"

using namespace daemon::interface;

uint32_t IWiimote::id() const { return m_id; }
void IWiimote::setId(const uint32_t &id) { m_id = id; }