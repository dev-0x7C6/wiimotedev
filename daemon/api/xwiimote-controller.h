#pragma once

#include <memory>
#include <string>

#include "interfaces/iwiimote-api.h"

struct xwii_iface;
struct xwii_event;

namespace daemon {
namespace api {

class XWiimoteController final : public interface::IWiimote {
public:
	explicit XWiimoteController(const std::string &interfaceFilePath);
	virtual ~XWiimoteController();

	virtual interface::ApiType type() const override { return interface::ApiType::XWiimote; }

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

private:
	std::string interfaceFilePath() const;

	void reconfigure();

	int process(xwii_event &event);

private:
	const std::string m_interfaceFilePath;
	xwii_iface *m_interface;
	int m_fd;
};
}
}
