#pragma once

#include <QObject>
#include <memory>
#include "interfaces/icontainer.h"

namespace service {
namespace interface {

class IContainerProcessor : public QObject {
public:
	explicit IContainerProcessor(QObject *parent = nullptr)
			: QObject(parent) {}
	virtual ~IContainerProcessor() = default;

	virtual void process(const uint32_t id, const std::unique_ptr<IContainer> &container) = 0;
};
}
}
