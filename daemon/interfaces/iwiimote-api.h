#pragma once

#include <cstdint>
#include <array>

namespace daemon {
namespace interface {

enum class ApiType {
	Cwiid,
	XWiimote
};

class IWiimote {
public:
	explicit IWiimote() = default;
	virtual ~IWiimote() = default;

	enum class Device {
		Wiimote,
		BalanceBoard,
		ProController
	};

	virtual ApiType type() const = 0;

	virtual bool isRumbleSupported() = 0;
	virtual bool isLedSupported() = 0;
	virtual bool isInfraredSupported() = 0;

	virtual uint8_t batteryStatus() = 0;
	virtual bool ledStatus(const uint32_t id) = 0;
	virtual bool rumbleStatus() = 0;

	virtual bool setLedStatus(const uint32_t id, const bool status) = 0;
	virtual bool setRumbleStatus(const bool rumble) = 0;

	virtual bool hasClassicExtension() = 0;
	virtual bool hasMotionPlusExtension() = 0;
	virtual bool hasNunchukExtension() = 0;

protected:
};
}
}
