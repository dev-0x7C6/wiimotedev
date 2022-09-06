#pragma once

#include <QObject>
#include <QPoint>
#include <QJsonObject>

class VirtualCursor final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QJsonObject ir READ ir NOTIFY irChanged)
	Q_PROPERTY(QJsonObject cursor READ cursor NOTIFY cursorChanged)
public:
	explicit VirtualCursor(QObject *parent = nullptr);

	auto cursor() const -> QJsonObject;
	auto ir() const -> QJsonObject;

private:
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void dataChanged(uint id, double x, double y, double yaw, double roll, double pitch, double distance_cm);
	void visibilityChanged(uint id, uint visible);

private:
	QJsonObject m_cursor;
	QJsonObject m_ir{};

signals:
	void cursorChanged(QJsonObject);
	void irChanged(QJsonObject);
};
