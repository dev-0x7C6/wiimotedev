#include "infrared-container.h"

using namespace daemon::container;

const IrPoints &InfraredContainer::points() const { return m_points; }

void InfraredContainer::setPoints(const IrPoints &points) { m_points = points; }
