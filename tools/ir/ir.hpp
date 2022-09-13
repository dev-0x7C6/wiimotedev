#pragma once

#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QPoint>

class VirtualCursor final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QJsonArray model READ model NOTIFY modelChanged)
public:
	explicit VirtualCursor(QObject *parent = nullptr);

	auto model() const -> QJsonArray;

private:
	void buttonDataChanged(uint id, qulonglong mask);
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm, bool visible);
	void visibilityChanged(uint id, bool visible);

private:
	QMap<std::uint32_t, QJsonObject> m_model;

signals:
	void modelChanged(QJsonArray);
};
