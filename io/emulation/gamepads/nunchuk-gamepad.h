#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class NunchukGamepad final : public IGamepad {
public:
	explicit NunchukGamepad(const std::string &name, const uint32_t id);
	virtual Type type() const override;

	virtual bool input(const uint64_t buttons) override;
	virtual bool input(const Stick stick, const int32_t x, const int32_t y) override;

	virtual bool configure() override;
	virtual bool centerAllAxis() override;

private:
	bool centerStick(Stick id);
	bool syncSticks();

signals:
	void setLedState(uint32_t, uint32_t);

private:
	int m_last_stick_x;
	int m_last_stick_y;
};
}
}
}
