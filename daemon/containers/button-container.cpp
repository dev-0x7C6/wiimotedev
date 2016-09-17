#include "button-container.h"

using namespace daemon::container;
using namespace daemon::interface;

IContainer::Type ButtonContainer::type() const { return Type::Button; }

IContainer::Source ButtonContainer::source() const { return Source::Wiimote; };
