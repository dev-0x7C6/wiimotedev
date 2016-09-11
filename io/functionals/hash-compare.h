#pragma once

#include <QObject>
#include <QHash>
#include <QHashIterator>

template <class typeKey, class typeValue>
class HashCompare {
public:
	explicit HashCompare() = default;
	virtual ~HashCompare() = default;

	enum CompareStyle {
		BitCompare = 0,
		EqualCompare,
		NotEqualCompare
	};

	bool compare(QHash<typeKey, typeValue> *, QHash<typeKey, typeValue> *, uint8_t);
};

template <class typeKey, class typeValue>
bool HashCompare<typeKey, typeValue>::compare(QHash<typeKey, typeValue> *first, QHash<typeKey, typeValue> *second, uint8_t style) {
	if (first->isEmpty() || second->isEmpty())
		return false;

	bool matched = true;
	QHashIterator<typeKey, typeValue> map(*first);

	while (map.hasNext()) {
		map.next();

		switch (style) {
			case HashCompare::BitCompare:
				matched &= ((map.value() & second->value(map.key(), 0)) == map.value());
				break;

			case HashCompare::EqualCompare:
				matched &= (map.value() == second->value(map.key(), 0));
				break;

			case HashCompare::NotEqualCompare:
				matched &= (map.value() != second->value(map.key(), 0));
				break;
		}

		if (!matched)
			break;
	}

	return matched;
}
