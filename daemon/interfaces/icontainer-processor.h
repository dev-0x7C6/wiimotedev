#pragma once

#include <memory>
#include <QObject>

#include "common/enums/device.h"
#include "enums/adaptor.h"
#include "interfaces/icontainer.h"

namespace dae {
namespace interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(QObject *parent = nullptr);
	virtual ~IContainerProcessor() = default;

	virtual enums::Adaptor type() const = 0;
	virtual void process(const common::enums::Device device, const uint32_t id, const std::unique_ptr<IContainer> &container) = 0;
};
}
}
