#include "button-container.h"

using namespace service::container;
using namespace service::interface;

IContainer::Type ButtonContainer::type() const { return Type::Button; }

uint64_t ButtonContainer::state() const {
	return m_state;
}

void ButtonContainer::setState(const uint64_t &state) {
	m_state = state;
}

ButtonContainer::ButtonContainer(const IContainer::Source source, const uint64_t state)
		: m_state(state)
		, m_source(source) {
}

IContainer::Source ButtonContainer::source() const { return Source::Wiimote; };
