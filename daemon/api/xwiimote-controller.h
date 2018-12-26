#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>

#include "interfaces/iwiimote-api.h"

enum class error_class {
	critical,
	error,
	warning,
	information,
	notice,
	hint,
	debug,
};

template <error_class filter_above = error_class::information, std::ostream &out = std::cout, std::ostream &err = std::cerr>
class logger {
public:
	template <error_class id = error_class::information, typename... args>
	constexpr static void log(args &&... values) {
		if constexpr (filter_above >= id) {
			switch (id) {
				case error_class::critical:
				case error_class::error:
				case error_class::warning:
					print_error(std::forward<args>(values)...);
					break;
				case error_class::information:
				case error_class::notice:
				case error_class::hint:
				case error_class::debug:
					print_standard(std::forward<args>(values)...);
					break;
			}
		}
	}

	template <typename... args>
	constexpr static void critical(args &&... values) {
		log<error_class::critical>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void error(args &&... values) {
		log<error_class::error>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void warning(args &&... values) {
		log<error_class::warning>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void information(args &&... values) {
		log<error_class::information>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void notice(args &&... values) {
		log<error_class::notice>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void hint(args &&... values) {
		log<error_class::hint>(std::forward<args>(values)...);
	}

	template <typename... args>
	constexpr static void debug(args &&... values) {
		log<error_class::debug>(std::forward<args>(values)...);
	}

protected:
	template <typename... args>
	constexpr static void print_error(args &&... values) {
		((out << values), ...) << std::endl;
	}

	template <typename... args>
	constexpr static void print_standard(args &&... values) {
		((err << values), ...) << std::endl;
	}
};

class call_on_destroy {
public:
	call_on_destroy(std::function<void()> &&callable)
			: m_callable(std::move(callable)) {}

	call_on_destroy(call_on_destroy &&rhs) noexcept = default;
	call_on_destroy(call_on_destroy const &) = delete;

	call_on_destroy &operator=(call_on_destroy &&) noexcept = default;
	call_on_destroy &operator=(call_on_destroy const &) = delete;
	call_on_destroy &operator=(std::function<void()> &&callable) noexcept {
		m_callable = callable;
		return *this;
	}

	~call_on_destroy() {
		if (m_callable)
			m_callable();
	}

private:
	std::function<void()> m_callable;
};

using destruction_queue = std::queue<call_on_destroy>;

struct xwii_iface;
struct xwii_event;

namespace dae {
namespace api {

class XWiimoteController final : public interface::IWiimote {
public:
	explicit XWiimoteController(interface::IIdManager &manager, std::string &&path);
	~XWiimoteController() override;

	common::enums::Device type() const final;
	Api api() const final { return Api::XWiimote; }

	bool isValid() const final;
	std::unique_ptr<interface::IContainer> process() override;

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
	destruction_queue m_destructionQueue;
	const std::string m_interfaceFilePath;
	xwii_iface *m_interface{nullptr};
	bool m_connected{false};
	int m_fd{0};
	std::array<u64, 5> m_buttons{0, 0, 0, 0, 0};

	std::queue<std::unique_ptr<interface::IContainer>> m_messages;
	logger<error_class::debug> m_logger;

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
