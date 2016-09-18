#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include "interfaces/icontainer.h"

namespace service {
namespace interface {

class IWiimote {
public:
	explicit IWiimote() = default;
	virtual ~IWiimote() = default;

	enum class Type {
		Wiimote,
		BalanceBoard,
		ProController
	};

	enum class Api {
		Cwiid,
		XWiimote
	};

	virtual Type type() const = 0;
	virtual Api api() const = 0;

	virtual std::unique_ptr<IContainer> process() = 0;

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

	uint32_t id() const;
	void setId(const uint32_t &id);

private:
	uint32_t m_id = 0u;
};
}
}
