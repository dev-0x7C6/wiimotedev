#include "status-container.h"

using namespace service::container;
using namespace service::interface;
using namespace service::enums;

StatusContainer::StatusContainer(const enums::Device source, const StatusContainer::State state)
		: m_source(source)
		, m_state(state) {
}

Event StatusContainer::event() const { return Event::Status; }
Device StatusContainer::device() const { return m_source; }
StatusContainer::State StatusContainer::state() const { return m_state; }
