#pragma once

#include <cstdint>
#include <string>

namespace io {
namespace interface {

class IInputDevice {
public:
	explicit IInputDevice(const std::string &name, const uint32_t id);
	virtual ~IInputDevice() = default;

	virtual bool open() = 0;
	virtual bool configure() = 0;
	virtual bool create() = 0;
	virtual bool destroy() = 0;
	virtual bool close() = 0;

	virtual uint32_t id() const;
	virtual std::string name() const;

	virtual bool isOpen() const = 0;
	virtual bool isCreated() const = 0;

protected:
	const std::string m_name;
	const uint32_t m_id;
};
}
}
