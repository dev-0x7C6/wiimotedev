#pragma once

#include <interfaces/igamepad.h>

namespace io {
namespace emulation {
namespace gamepad {

class ClassicGamepad final : public IGamepad {
public:
	explicit ClassicGamepad(const std::string &name, const uint32_t id);

	virtual Type type() const override;

	virtual bool input(const uint64_t buttons) override;
	virtual bool input(const Stick stick, const int32_t x, const int32_t y) override;

	virtual bool configure() override;
	virtual bool centerAllAxis() override;

private:
	void centerStick(Stick stick);
	void syncSticks();

private:
	std::array<int16_t, 6> m_axis;
};
}
}
}
