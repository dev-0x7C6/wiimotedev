#pragma once

#include <functional>
#include <memory>

#include <QObject>

#include "common/enums/command-type.hpp"
#include "common/enums/device.h"
#include "enums/adaptor.h"
#include "interfaces/icontainer.h"

namespace dae::interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(EventCallback &&eventCallback, QObject *parent = nullptr);
	~IContainerProcessor() override = default;

	virtual enums::Adaptor type() const = 0;
	virtual void process(common::enums::Device device, u32 id, const std::unique_ptr<IContainer> &container) = 0;

protected:
	CommandResult generateEvent(CommandEvent &&event) const noexcept;

private:
	EventCallback m_eventCallback;
};
}
