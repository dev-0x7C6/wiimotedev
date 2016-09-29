#include "status-container.h"

using namespace service::container;
using namespace service::interface;
using namespace service::enums;

StatusContainer::StatusContainer(const enums::Device source, const StatusContainer::State state)
		: m_source(source)
		, m_state(state) {
}

IContainer::Type StatusContainer::type() const { return Type::Status; }
Device StatusContainer::deviceType() const { return m_source; }
StatusContainer::State StatusContainer::state() const { return m_state; }
