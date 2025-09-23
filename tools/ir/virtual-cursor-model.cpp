#include "virtual-cursor-model.hpp"
#include "dbus/interfaces/virtualcursor.h"
#include "dbus/interfaces/wiimote.h"

#include <QDBusConnection>
#include <include/wiimotedev/wiimotedev>

VirtualCursorModel::VirtualCursorModel(QObject *parent)
        : QAbstractListModel(parent) {
    for (auto &&bus : {QDBusConnection::systemBus(), QDBusConnection::sessionBus()}) {
        auto wiimote = new org::wiimotedev::wiimote("org.wiimotedev.daemon", "/wiimote", bus, this);
        auto cursor = new org::wiimotedev::virtualcursor("org.wiimotedev.daemon", "/virtualcursor", bus, this);
        connect(wiimote, &org::wiimotedev::wiimote::buttonDataChanged, this, &VirtualCursorModel::buttonDataChanged);
        connect(cursor, &org::wiimotedev::virtualcursor::dataChanged, this, &VirtualCursorModel::virtualCursorDataChanged);
    }
}

auto VirtualCursorModel::getState(int id) -> VirtualCursorState * {
    const auto st = std::ranges::find_if(m_virtual_cursor_state, [id](const VirtualCursorState &state) {
        return (state.id == id);
    });

    if (st != std::end(m_virtual_cursor_state))
        return &*st;

    VirtualCursorState state;
    state.id = id;
    m_virtual_cursor_state.emplace_back(std::move(state));

    return &m_virtual_cursor_state.back();
}

auto VirtualCursorModel::getStateIndex(int id) -> std::optional<int> {
    const auto st = std::ranges::find_if(m_virtual_cursor_state, [id](const VirtualCursorState &state) {
        return (state.id == id);
    });

    if (st == std::end(m_virtual_cursor_state))
        return {};

    return std::distance(std::begin(m_virtual_cursor_state), st);
}

auto VirtualCursorModel::virtualCursorDataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible) -> void {
    auto last = *getState(id);
    auto state = getState(id);
    state->x = x;
    state->y = y;
    state->yaw = yaw;
    state->roll = roll;
    state->pitch = pitch;
    state->distance = distance_cm;
    state->visible = visible;

    update(id);

    if (last != *state) {
        const auto idx = getStateIndex(id);
        if (idx)
            dataChanged(index(idx.value()), index(idx.value()), {CursorRole});
    }
}

auto VirtualCursorModel::buttonDataChanged(uint id, qulonglong mask) -> void {
    using namespace wiimotedev;

    const auto A_PRESSED = (mask & WIIMOTEDEV_BTN_A) == WIIMOTEDEV_BTN_A;
    const auto B_PRESSED = (mask & WIIMOTEDEV_BTN_B) == WIIMOTEDEV_BTN_B;

    getState(id)->press = A_PRESSED && B_PRESSED;
    update(id);
}

auto VirtualCursorModel::rowCount(const QModelIndex &) const -> int {
    return m_virtual_cursor_state.size();
}

auto VirtualCursorModel::data(const QModelIndex &index, int role) const -> QVariant {
    if (!index.isValid())
        return {};

    if (index.row() >= rowCount({}))
        return {};

    if (CursorRole != role)
        return {};

    const auto &state = m_virtual_cursor_state.at(index.row());

    QVariantMap data;
    data["id"] = state.id;
    data["x"] = state.x;
    data["y"] = state.y;
    data["yaw"] = state.yaw;
    data["roll"] = state.roll;
    data["pitch"] = state.pitch;
    data["distance"] = state.distance;
    data["visible"] = state.visible;
    data["press"] = state.press;

    return data;
}

auto VirtualCursorModel::roleNames() const -> QHash<int, QByteArray> {
    return {
        {CursorRole, "cursor"},
    };
}

auto VirtualCursorModel::update(const int id) -> void {
    if (rowCount({}) == 0)
        return;

    const auto rows = rowCount({});
    const auto offset = std::max(m_count, 0);

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
