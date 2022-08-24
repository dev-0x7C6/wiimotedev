#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"
#include "containers/structs.hpp"

namespace dae {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ClassicDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(common::enums::Device device, u32 id, const dae::container::structs::event &ev) final;

public:
	QList<uint> list() const;

private:
	QSet<uint> m_ids;

signals:
	void buttonDataChanged(uint id, qulonglong mask);
	void connected(uint id);
	void disconnected(uint id);
	void stickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
