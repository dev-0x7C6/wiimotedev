#include "button-container.h"

using namespace service::container;
using namespace service::interface;

IContainer::Type ButtonContainer::type() const { return Type::Button; }

IContainer::Source ButtonContainer::source() const { return Source::Wiimote; };
