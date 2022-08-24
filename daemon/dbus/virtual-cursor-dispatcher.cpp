#include "virtual-cursor-dispatcher.h"
#include "virtualcursoradaptor.h"

#include "containers/infrared-container.h"
#include "functionals/virtual-cursor-processor.h"

using namespace common::enums;
using namespace dae::container;
using namespace dae::dbus;
using namespace dae::enums;
using namespace dae::functional;

VirtualCursorDispatcher::VirtualCursorDispatcher(EventCallback &&eventCallback)
		: IContainerProcessor(std::move(eventCallback)) {
	new VirtualcursorAdaptor(this);
}

VirtualCursorDispatcher::~VirtualCursorDispatcher() = default;

Adaptor VirtualCursorDispatcher::type() const { return Adaptor::VirtualCursor; }
QList<uint> VirtualCursorDispatcher::list() const { return m_ids.values(); }

void VirtualCursorDispatcher::process(const Device device, const u32 id, const dae::container::structs::event &ev) {
	if (Device::Wiimote != device)
		return;

	//	if (Event::Infrared == container->event()) {
	//		const auto &ir = static_cast<InfraredContainer *>(container.get())->points();

	//		if (!m_processors.count(id)) {
	//			m_processors.emplace(id, std::make_unique<VirtualCursorProcessor>());
	//		}

	//		QList<QPair<int, int>> points;
	//		for (const auto &point : ir)
	//			if (point.x != 1023 && point.y != 1023)
	//				points.append({point.x, point.y});

	//		auto &cursor = m_processors.at(id);

	//		auto visibleAfter = cursor->isVisible();
	//		bool isValid = cursor->calculate(points, 0);

	//		if (visibleAfter && !cursor->isVisible())
	//			emit lost(id);

	//		if (!visibleAfter && cursor->isVisible())
	//			emit found(id);

	//		if (isValid)
	//			emit dataChanged(id, cursor->x(), cursor->y(), cursor->distance(), cursor->angle());
	//	}
}
