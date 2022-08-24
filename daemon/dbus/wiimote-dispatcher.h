#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class WiimoteDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit WiimoteDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(u32 id, const dae::container::structs::event &ev) final;

public Q_SLOTS:
	QList<uint> list();
	bool ledStatus(uint id, uint led_id);
	bool rumbleStatus(uint id);
	bool setLedStatus(uint id, uint led_id, bool status);
	bool setRumbleStatus(uint id, bool status);

signals:
	void connected(uint id);
	void disconnected(uint id);

	void accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void buttonDataChanged(uint id, qulonglong mask);
	void gyroscopeDataChanged(uint id, int x, int y, int z);
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

private:
	QSet<uint> m_ids;
};
}
}
