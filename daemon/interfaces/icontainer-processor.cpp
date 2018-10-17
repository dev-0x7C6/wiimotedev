#include "icontainer-processor.h"

using namespace common::enums;
using namespace dae::interface;
using namespace dae::enums;

IContainerProcessor::IContainerProcessor(EventCallback &&eventCallback, QObject *parent)
		: QObject(parent)
		, m_eventCallback(std::move(eventCallback)) {}

CommandResult IContainerProcessor::generateEvent(CommandEvent &&event) const noexcept {
	return m_eventCallback(std::move(event));
}
