#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>

#include <externals/common/logger/logger.hpp>
#include <externals/common/std/raii/raii-tail-call.hpp>

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

	common::enums::Device type() const final;
	Api api() const final { return Api::XWiimote; }

	bool isValid() const final;
	std::vector<std::unique_ptr<interface::IContainer>> process() override;

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
	bool m_connected{false};
	std::array<u64, 5> m_buttons{0, 0, 0, 0, 0};

	logger<error_class::debug> m_logger;
	std::unique_ptr<::helper::xwii_iface_instance> instance;
	std::unique_ptr<::helper::xwii_iface_session> session;

	std::optional<bool> m_balanceBoardConnected;
	std::optional<bool> m_classicControllerConnected;
	std::optional<bool> m_motionPlusConnected;
	std::optional<bool> m_nunchukConnected;
	std::optional<bool> m_proControllerConnected;
	std::optional<bool> m_wiimoteConnected;

	bool m_rumbleStatus{false};
};
}
}
