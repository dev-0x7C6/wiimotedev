#pragma once

#include <array>
#include <memory>
#include <string>

#include "interfaces/iwiimote-api.h"

struct xwii_iface;
struct xwii_event;

namespace service {
namespace api {

class XWiimoteController final : public interface::IWiimote {
public:
	explicit XWiimoteController(const std::string &interfaceFilePath);
	virtual ~XWiimoteController();

	virtual Type type() const override;
	virtual Api api() const override { return Api::XWiimote; }

	virtual bool isValid() const override;
	virtual std::unique_ptr<interface::IContainer> process() override;

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

private:
	const std::string m_interfaceFilePath;
	xwii_iface *m_interface;
	bool m_connected;
	int m_fd;
	std::array<uint64_t, 5> m_buttons;
};
}
}
