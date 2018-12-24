#pragma once

#include <array>

namespace dae {
namespace functional {

template <typename type, int size>
class UniqueId final {
public:
	decltype(size) reserve() {
		decltype(size) index = 1;
		while (size > index && m_table[index]) ++index;

		if (index >= size)
			return 0;

		m_table[index] = true;
		return index;
	}

	decltype(size) release(const decltype(size) index) {
		m_table[index] = false;
		return index;
	}

private:
	std::array<type, size> m_table{};
};
}
}
