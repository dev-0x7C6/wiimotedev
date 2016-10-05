#pragma once

#include <QObject>
#include <memory>
#include "interfaces/icontainer.h"
#include "common/enums/device.h"

namespace dae {
namespace interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(QObject *parent = nullptr);
	virtual ~IContainerProcessor() = default;

	virtual common::enums::Device device() const = 0;
	virtual std::string interface() const;

	virtual void process(const uint32_t id, const std::unique_ptr<IContainer> &container) = 0;
};
}
}
