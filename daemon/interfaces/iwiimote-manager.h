#pragma once

#include "interfaces/iwiimote-api.h"

#include <memory>

namespace daemon {
namespace interface {

class IWiimote;

class IWiimoteManager {
public:
	explicit IWiimoteManager() = default;
	virtual ~IWiimoteManager() = default;

	virtual std::unique_ptr<IWiimote> connect() = 0;
};
}
}
