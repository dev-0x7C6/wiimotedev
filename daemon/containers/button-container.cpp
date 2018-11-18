#include "button-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::interface;
using namespace dae::enums;

Event ButtonContainer::event() const { return Event::Button; }

u64 ButtonContainer::state() const {
	return m_state;
}

void ButtonContainer::setState(const u64 &state) {
	m_state = state;
}

ButtonContainer::ButtonContainer(const Device source, const u64 state)
		: m_state(state)
		, m_source(source) {
}

Device ButtonContainer::device() const { return m_source; }
