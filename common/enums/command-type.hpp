#pragma once

#include <functional>
#include <variant>
#include <optional>

#include <externals/common/types.hpp>

enum class CommandType {
	GetLedState,
	GetRumbleState,
	SetLedState,
	SetRumbleState,
};

struct GetLedStateEvent {
	u32 led_id;
};

struct SetRumbleStateEvent {
	u32 state;
};

struct SetLedStateEvent {
	u32 led_id;
	u32 state;
};

using CommandParameter = std::variant<std::monostate, GetLedStateEvent, SetRumbleStateEvent, SetLedStateEvent>;
using CommandResult = std::optional<std::variant<bool, u32>>;

class CommandEvent {
public:
	constexpr CommandEvent(CommandType command, u32 id, CommandParameter parameter)
			: m_command(command)
			, m_id(id)
			, m_parameter(parameter) {}

	constexpr auto command() const noexcept { return m_command; }
	constexpr auto id() const noexcept { return m_id; }
	constexpr auto parameter() const noexcept { return m_parameter; }

private:
	CommandType m_command;
	u32 m_id{0};
	CommandParameter m_parameter;
};

using EventCallback = std::function<CommandResult(CommandEvent &&)>;
