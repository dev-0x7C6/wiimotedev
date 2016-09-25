#pragma once

#include <memory>
#include <QObject>

#include "interfaces/icontainer-processor.h"

class WiimoteAdaptor;

namespace service {
namespace dbus {

class Wiimote final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit Wiimote(QObject *parent = nullptr);

	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

	QList<uint> wiimoteList() const;

signals:
	void wiimoteConnected(uint id);
	void wiimoteDisconnected(uint id);

	void wiimoteAccelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void wiimoteButtonDataChanged(uint id, qulonglong mask);
	void wiimoteGyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ);
	void wiimoteInfraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
};
}
}
