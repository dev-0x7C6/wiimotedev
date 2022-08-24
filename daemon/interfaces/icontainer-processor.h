#pragma once

#include <functional>
#include <memory>

#include <QObject>

#include "common/enums/command-type.hpp"
#include "common/enums/device.h"
#include "enums/adaptor.h"
#include "containers/structs.hpp"

namespace dae::interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(EventCallback &&eventCallback, QObject *parent = nullptr);
	~IContainerProcessor() override = default;

	virtual enums::Adaptor type() const = 0;
	virtual void process(common::enums::Device device, u32 id, const dae::container::structs::event &ev) = 0;

protected:
	CommandResult generateEvent(CommandEvent &&event) const noexcept;

private:
	EventCallback m_eventCallback;
};
}
