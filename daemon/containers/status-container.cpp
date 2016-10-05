#include "status-container.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::enums;
using namespace dae::interface;

StatusContainer::StatusContainer(const common::enums::Device source, const StatusContainer::State state)
		: m_source(source)
		, m_state(state) {
}

Event StatusContainer::event() const { return Event::Status; }
Device StatusContainer::device() const { return m_source; }
StatusContainer::State StatusContainer::state() const { return m_state; }
