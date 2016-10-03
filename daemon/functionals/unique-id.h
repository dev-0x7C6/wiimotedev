#pragma once

#include <array>

namespace service {
namespace functional {

template <typename type, std::size_t size>
class UniqueId final {
public:
	explicit UniqueId() {
		for (int i = 0; i < size; ++i) m_table[i] = false;
	}
	virtual ~UniqueId() = default;

	decltype(size) take() {
		decltype(size) index = 1;
		while (size > index && m_table[index]) ++index;

		if (index >= size)
			return 0;

		m_table[index] = true;
		return index;
	}

	decltype(size) debt(const decltype(size) index) {
		m_table[index] = false;
		return index;
	}

private:
	std::array<type, size> m_table;
};
}
}
