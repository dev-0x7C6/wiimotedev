#pragma once

#include "interfaces/iwiimote-api.h"

namespace daemon {
namespace api {

class CwiidApi final : public interface::IWiimoteApi {
public:
	explicit CwiidApi() = default;
	virtual ~CwiidApi() = default;

	virtual bool isRumbleSupported() override;
	virtual bool isLedSupported() override;
	virtual bool isInfraredSupported() override;

	virtual uint8_t batteryStatus() override;
	virtual bool ledStatus(const uint32_t id) override;
	virtual bool rumbleStatus() override;

	virtual bool setLedStatus(const uint32_t id, const bool status) override;
	virtual bool setRumbleStatus(const bool rumble) override;
	virtual bool hasClassicExtension() override;
	virtual bool hasMotionPlusExtension() override;
	virtual bool hasNunchukExtension() override;
};
}
}
