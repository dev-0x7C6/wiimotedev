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
	void process(common::enums::Device device, u32 id, const std::unique_ptr<interface::IContainer> &container) final;

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
