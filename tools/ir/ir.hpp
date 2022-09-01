#pragma once

#include <QObject>
#include <QPoint>
#include <QJsonObject>

class VirtualCursor final : public QObject {
	Q_OBJECT
	Q_PROPERTY(QJsonObject ir READ ir NOTIFY irChanged)
	Q_PROPERTY(QPointF cursor READ cursor NOTIFY cursorChanged)
	Q_PROPERTY(double angle READ angle NOTIFY angleChanged)
public:
	explicit VirtualCursor(QObject *parent = nullptr);

	auto cursor() const -> QPointF;
	auto ir() const -> QJsonObject;
	auto angle() const -> double { return m_angle; };

private:
	void infraredDataChanged(uint id, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void dataChanged(uint id, double x, double y, double l, double a);

private:
	QPointF m_cursor;
	double m_angle{};
	QJsonObject m_ir{};

signals:
	void cursorChanged(QPointF);
	void angleChanged(double);
	void irChanged(QJsonObject);
};
