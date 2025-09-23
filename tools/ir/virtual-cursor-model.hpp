#pragma once

#include <QAbstractListModel>

class VirtualCursorModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit VirtualCursorModel(QObject *parent = nullptr);

private:
    constexpr static auto CursorRole = Qt::UserRole + 1;

    auto data(const QModelIndex &index, int role = Qt::DisplayRole) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;

    auto buttonDataChanged(uint id, qulonglong mask) -> void;
    auto virtualCursorDataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible) -> void;

private:
    struct VirtualCursorState {
        int id{};
        int x{};
        int y{};
        int distance{};
        double yaw{};
        double roll{};
        double pitch{};
        bool visible{false};
        bool press{false};

        constexpr auto operator<=>(const VirtualCursorState &) const noexcept = default;
    };

    auto getState(int id) -> VirtualCursorState *;
    auto getStateIndex(int id) -> std::optional<int>;
    auto update(const int id) -> void;

private:
    std::vector<VirtualCursorState> m_virtual_cursor_state;
    int m_count{-1};
};
