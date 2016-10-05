#pragma once

#include "interfaces/iwiimote-api.h"

#include <memory>

namespace dae {
namespace interface {

class IWiimote;

class IWiimoteManager {
public:
	explicit IWiimoteManager() = default;
	virtual ~IWiimoteManager() = default;

	virtual IWiimote::Api api() const = 0;
	virtual std::unique_ptr<IWiimote> connect() = 0;
};
}
}
