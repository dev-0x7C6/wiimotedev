#pragma once

#include <QAbstractListModel>

class InfraredModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit InfraredModel(QObject *parent = nullptr);

private:
    constexpr static auto ir_role = Qt::UserRole + 1;

    auto data(const QModelIndex &index, int role = Qt::DisplayRole) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;

    auto buttonDataChanged(uint id, qulonglong mask) -> void;
    auto infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) -> void;

private:
    struct IrPoint {
        int id{-1};
        int num{-1};
        int x{};
        int y{};
        bool visible{false};
        constexpr auto operator<=>(const IrPoint &) const noexcept = default;
    };

    auto getState(const int id, const int num) -> IrPoint *;
    auto getStateIndex(const int id, const int num) -> std::optional<int>;
    auto update() -> void;

private:
    std::vector<IrPoint> m_points;
    int m_count{-1};
};
