#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include "interfaces/icontainer.h"
#include "interfaces/iid-manager.h"
#include <externals/common/types.hpp>

namespace dae {
namespace interface {

class IWiimote {
public:
	explicit IWiimote(IIdManager &manager);
	virtual ~IWiimote() = default;

	enum class Api {
		XWiimote
	};

	virtual common::enums::Device type() const = 0;
	virtual Api api() const = 0;

	virtual bool isValid() const = 0;
	virtual std::unique_ptr<IContainer> process() = 0;

	virtual bool isRumbleSupported() = 0;
	virtual bool isLedSupported() = 0;
	virtual bool isInfraredSupported() = 0;

	virtual u8 batteryStatus() = 0;
	virtual bool ledStatus(u32 id) = 0;
	virtual bool rumbleStatus() = 0;

	virtual bool setLedStatus(const u32 id, const bool status) = 0;
	virtual bool setRumbleStatus(const bool rumble) = 0;

	virtual bool hasClassicExtension() = 0;
	virtual bool hasMotionPlusExtension() = 0;
	virtual bool hasNunchukExtension() = 0;

	u32 id() const;
	void setId(const u32 &id);

protected:
	IIdManager &m_idManager;

private:
	u32 m_id = 0u;
};
}
}
