#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class NunchukDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit NunchukDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(u32 id, const dae::container::event &ev) final;

public:
	QList<uint> list() const;

signals:
	void accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void buttonDataChanged(uint id, qulonglong mask);
	void connectionChanged(uint id, bool connected);
	void stickDataChanged(uint id, int x, int y);

private:
	QSet<uint> m_ids;
};
}
}
