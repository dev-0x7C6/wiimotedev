#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class WiimoteDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit WiimoteDispatcher(QObject *parent = nullptr);
	virtual ~WiimoteDispatcher() = default;

	virtual enums::Adaptor type() const override;
	virtual void process(const common::enums::Device, const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list() const;

signals:
	void connected(uint id);
	void disconnected(uint id);

	void accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void buttonDataChanged(uint id, qulonglong mask);
	void gyroscopeDataChanged(uint id, int x, int y, int z, int lowX, int lowY, int lowZ);
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

private:
	QSet<uint> m_ids;
};
}
}
