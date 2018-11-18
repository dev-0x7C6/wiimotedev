#pragma once

#include <functional>
#include <memory>

#include <QObject>

#include "common/enums/device.h"
#include "enums/adaptor.h"
#include "interfaces/icontainer.h"

namespace dae::interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(EventCallback &&eventCallback, QObject *parent = nullptr);
	virtual ~IContainerProcessor() = default;

	virtual enums::Adaptor type() const = 0;
	virtual void process(const common::enums::Device device, const u32 id, const std::unique_ptr<IContainer> &container) = 0;

protected:
	CommandResult generateEvent(CommandEvent &&event) const noexcept;

private:
	EventCallback m_eventCallback;
};
}
