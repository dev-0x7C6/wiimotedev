#pragma once

#include <cstdint>
#include <string>

#include <externals/common/types.hpp>

namespace io {
namespace interface {

class IInputDevice {
public:
	explicit IInputDevice(std::string &&name, u32 id);

	IInputDevice(const IInputDevice &) = delete;
	IInputDevice &operator=(const IInputDevice &) = delete;
	IInputDevice(IInputDevice &&) noexcept = delete;
	IInputDevice &operator=(IInputDevice &&) noexcept = delete;

	virtual ~IInputDevice() = default;

	virtual bool open() = 0;
	virtual bool configure() = 0;
	virtual bool create() = 0;
	virtual bool destroy() = 0;
	virtual bool close() = 0;

	auto id() const noexcept { return m_id; };
	auto name() const noexcept { return m_name; };

	virtual bool isOpen() const = 0;
	virtual bool isCreated() const = 0;

protected:
	const std::string m_name;
	const uint32_t m_id;
};
}
}
