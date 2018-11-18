#pragma once

#include <array>
#include <memory>
#include <string>
#include <queue>

#include "interfaces/iwiimote-api.h"

struct xwii_iface;
struct xwii_event;

namespace dae {
namespace api {

class XWiimoteController final : public interface::IWiimote {
public:
	explicit XWiimoteController(interface::IIdManager &manager, const std::string &interfaceFilePath);
	~XWiimoteController() override;

	virtual common::enums::Device type() const override;
	virtual Api api() const override { return Api::XWiimote; }

	virtual bool isValid() const override;
	virtual std::unique_ptr<interface::IContainer> process() override;

	virtual bool isRumbleSupported() override;
	virtual bool isLedSupported() override;
	virtual bool isInfraredSupported() override;

	virtual u8 batteryStatus() override;
	virtual bool ledStatus(const u32 id) override;
	virtual bool rumbleStatus() override;

	virtual bool setLedStatus(const u32 id, const bool status) override;
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
	std::array<u64, 5> m_buttons;

	std::queue<std::unique_ptr<interface::IContainer>> m_messages;

	bool m_connectedFlag{false};
	bool m_balanceBoardConnected{false};
	bool m_classicControllerConnected{false};
	bool m_motionPlusConnected{false};
	bool m_nunchukConnected{false};
	bool m_proControllerConnected{false};
	bool m_wiimoteConnected{false};
	bool m_rumbleStatus{false};
};
}
}
