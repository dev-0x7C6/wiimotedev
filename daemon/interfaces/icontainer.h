#pragma once

namespace daemon {
namespace interface {

class IContainer {
public:
	enum class Type {
		Empty,
		Infrared,
		Button,
		Stick,
		Accelerometer,
		Gyroscope,
		Pressure,
	};

	enum class Source {
		Wiimote,
		Nunchuk,
		Classic,
		BalanceBoard,
		ProController,
	};

	explicit IContainer() = default;
	virtual ~IContainer() = default;

	virtual Type type() const = 0;
	virtual Source source() const = 0;
};
}
}
