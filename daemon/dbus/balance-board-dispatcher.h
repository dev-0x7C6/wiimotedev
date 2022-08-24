#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class BalanceBoardDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit BalanceBoardDispatcher(EventCallback &&eventCallback);

	enums::Adaptor type() const final;
	void process(common::enums::Device device, u32 id, const dae::container::structs::event &ev) final;

public:
	QList<uint> list();

signals:
	void connected(uint id);
	void dataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right);
	void disconnected(uint id);

private:
	QSet<uint> m_ids;
};
}
}
