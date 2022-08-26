#pragma once

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <queue>

#include "interfaces/iwiimote-api.h"

struct xwii_iface;
struct xwii_event;

namespace helper {
struct xwii_iface_instance;
struct xwii_iface_session;
}

namespace dae {
namespace api {

class XWiimoteController final : public interface::IWiimote {
public:
	explicit XWiimoteController(interface::IIdManager &manager, std::string &&path);
	~XWiimoteController() final;

	common::enums::device type() const final;
	Api api() const final { return Api::XWiimote; }

	bool isValid() const final;
	dae::container::events process() final;

	bool isRumbleSupported() final;
	bool isLedSupported() final;
	bool isInfraredSupported() final;

	u8 batteryStatus() final;
	bool ledStatus(u32 id) final;
	bool rumbleStatus() final;

	bool setLedStatus(u32 id, bool status) final;
	bool setRumbleStatus(bool rumble) final;

	bool hasClassicExtension() final;
	bool hasMotionPlusExtension() final;
	bool hasNunchukExtension() final;

private:
	bool openXWiimoteInterface();
	bool watchXWiimoteEvents();
	bool reconfigureXWiimoteInterface();

	std::string interfaceFilePath() const;

private:
	const std::string m_interfaceFilePath;
	xwii_iface *m_interface{nullptr};

	std::unique_ptr<::helper::xwii_iface_instance> instance;
	std::unique_ptr<::helper::xwii_iface_session> session;

	std::queue<container::event> queue;
	std::array<u64, common::enums::devices.size()> m_buttons{};
	std::array<bool, common::enums::devices.size()> currentExtensionTable{};
	std::array<bool, common::enums::devices.size()> lastExtensionTable{};

	std::optional<bool> m_balanceBoardConnected;
	std::optional<bool> m_classicControllerConnected;
	std::optional<bool> m_motionPlusConnected;
	std::optional<bool> m_nunchukConnected;
	std::optional<bool> m_proControllerConnected;
	std::optional<bool> m_wiimoteConnected;

	bool m_connected{false};
	bool m_rumbleStatus{false};
};
}
}
