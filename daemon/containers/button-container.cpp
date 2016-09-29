#include "button-container.h"

using namespace service::container;
using namespace service::interface;
using namespace service::enums;

IContainer::Type ButtonContainer::type() const { return Type::Button; }

uint64_t ButtonContainer::state() const {
	return m_state;
}

void ButtonContainer::setState(const uint64_t &state) {
	m_state = state;
}

ButtonContainer::ButtonContainer(const Device source, const uint64_t state)
		: m_state(state)
		, m_source(source) {
}

Device ButtonContainer::deviceType() const { return m_source; };
