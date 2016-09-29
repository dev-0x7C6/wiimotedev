#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace service {
namespace dbus {

class NunchukDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit NunchukDispatcher(QObject *parent = nullptr);

	virtual enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> nunchukList() const;

signals:
	void nunchukAccelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void nunchukButtonDataChanged(uint id, qulonglong mask);
	void nunchukConnected(uint id);
	void nunchukDisconnected(uint id);
	void nunchukStickDataChanged(uint id, int x, int y);

private:
	QSet<uint> m_ids;
};
}
}
