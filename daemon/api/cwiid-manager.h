#pragma once

#include "interfaces/iwiimote-manager.h"

namespace daemon {
namespace api {

class CwiidManager final : public interface::IWiimoteManager {
public:
	explicit CwiidManager() = default;
	virtual ~CwiidManager() = default;

	virtual interface::IWiimote::Api api() const override { return interface::IWiimote::Api::Cwiid; }
	virtual std::unique_ptr<interface::IWiimote> connect() override;
};
}
}
