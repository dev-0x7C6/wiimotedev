#include "icontainer.h"

using namespace daemon::interface;

IContainer::IContainer(const Source source)
		: m_source(source) {}

IContainer::Source IContainer::source() const { return m_source; }
