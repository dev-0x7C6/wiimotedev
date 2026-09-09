#include "infrared-model.hpp"
#include "dbus/interfaces/wiimote.h"

#include <QDBusConnection>

#include <include/wiimotedev/wiimotedev>

InfraredModel::InfraredModel(QObject *parent)
        : QAbstractListModel(parent) {
    for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
        auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", bus, this);
        connect(wiimote, &org::wiimotedev::wiimote::infraredDataChanged, this, &InfraredModel::infraredDataChanged);
    }
}

auto InfraredModel::getState(const int id, const int num) -> IrPoint * {
    auto item = std::ranges::find_if(m_points, [&](IrPoint &data) {
        return (data.id == id) && (data.num == num);
    });

    if (item != std::end(m_points))
        return &*item;

    m_points.emplace_back(IrPoint{
        .id = id,
        .num = num,
    });

    return &m_points.back();
}

auto InfraredModel::getStateIndex(const int id, const int num) -> std::optional<int> {
    const auto it = std::ranges::find_if(m_points, [&](IrPoint &data) {
        return (data.id == id) && (data.num == num);
    });

    if (it == std::end(m_points))
        return {};

    return std::distance(std::begin(m_points), it);
}

auto InfraredModel::infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) -> void {
    auto is_visible = [](auto x, auto y) {
        return x != 1023 && y != 1023;
    };

    std::array<IrPoint *, 4> points{
        getState(id, 0),
        getState(id, 1),
        getState(id, 2),
        getState(id, 3),
    };

    std::array<IrPoint, 4> last_points{
        *points[0],
        *points[1],
        *points[2],
        *points[3],
    };

    points[0]->x = x1;
    points[0]->y = y1;
    points[0]->visible = is_visible(x1, y1);
    points[1]->x = x2;
    points[1]->y = y2;
    points[1]->visible = is_visible(x2, y2);
    points[2]->x = x3;
    points[2]->y = y3;
    points[2]->visible = is_visible(x3, y3);
    points[3]->x = x4;
    points[3]->y = y4;
    points[3]->visible = is_visible(x4, y4);

    update();

	for (auto i = 0u; i < points.size(); ++i)
		if (last_points[i] != *points[i]) {
			const auto idx = index(getStateIndex(id, i).value_or(0));
			dataChanged(idx, idx, {ir_role});
		}
}

auto InfraredModel::rowCount(const QModelIndex &) const -> int {
    return m_points.size();
}

auto InfraredModel::data(const QModelIndex &index, int role) const -> QVariant {
    if (!index.isValid())
        return {};

    if (index.row() >= rowCount({}))
        return {};

    if (ir_role != role)
        return {};

    const auto &point = m_points.at(index.row());

    QVariantMap data;
    data["id"] = point.id;
    data["num"] = point.num;
    data["x"] = point.x;
    data["y"] = point.y;
    data["visible"] = point.visible;

    return data;
}

auto InfraredModel::roleNames() const -> QHash<int, QByteArray> {
    return {
        {ir_role, "infrared"},
    };
}

auto InfraredModel::update() -> void {
    const auto rows = rowCount({});
    const auto offset = std::max(m_count - 1, 0);

    if (rows > m_count) {
        beginInsertRows(QModelIndex(), offset, rows - 1);
        endInsertRows();
    }

    if (rows < m_count) {
        beginRemoveRows(QModelIndex(), rows - 1, offset);
        endRemoveRows();
    }

    m_count = rows;
}
