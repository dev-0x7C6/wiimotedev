#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class NunchukDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit NunchukDispatcher(QObject *parent = nullptr);

	virtual common::enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> list() const;

signals:
	void accelerometerDataChanged(uint id, int x, int y, int z, int pitch, int roll);
	void buttonDataChanged(uint id, qulonglong mask);
	void connected(uint id);
	void disconnected(uint id);
	void stickDataChanged(uint id, int x, int y);

private:
	QSet<uint> m_ids;
};
}
}
