#pragma once

#include "interfaces/iwiimote-manager.h"

namespace daemon {
namespace api {

class CwiidManager final : public interface::IWiimoteManager {
public:
	explicit CwiidManager() = default;
	virtual ~CwiidManager() = default;

	virtual std::unique_ptr<interface::IWiimote> connect() override;
};
}
}
