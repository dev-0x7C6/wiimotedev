#pragma once

#include <QSet>
#include "interfaces/icontainer-processor.h"

namespace dae {
namespace dbus {

class ClassicDispatcher final : public interface::IContainerProcessor {
	Q_OBJECT
public:
	explicit ClassicDispatcher(QObject *parent = nullptr);
	virtual ~ClassicDispatcher() = default;

	virtual common::enums::Device device() const override;
	virtual void process(const uint32_t id, const std::unique_ptr<interface::IContainer> &container) override;

public:
	QList<uint> classicList() const;

private:
	QSet<uint> m_ids;

signals:
	void classicButtonDataChanged(uint id, qulonglong mask);
	void classicConnected(uint id);
	void classicDisconnected(uint id);
	void classicStickDataChanged(uint id, int lx, int ly, int rx, int ry);
};
}
}
