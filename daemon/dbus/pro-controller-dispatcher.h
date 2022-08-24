#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ProControllerDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ProControllerDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(u32 id, const dae::container::structs::event &ev) final;

public:
	QList<uint> list();

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connected(uint id);
	void disconnected(uint id);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);

private:
	QSet<uint> m_ids;
};
}
}
