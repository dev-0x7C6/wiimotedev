#pragma once

#include <QObject>

class BalanceBoard final : public QObject {
	Q_OBJECT
	Q_PROPERTY(int tl READ topLeft NOTIFY topLeftChanged)
	Q_PROPERTY(int tr READ topRight NOTIFY topRightChanged)
	Q_PROPERTY(int bl READ bottomLeft NOTIFY bottomLeftChanged)
	Q_PROPERTY(int br READ bottomRight NOTIFY bottomRightChanged)
public:
	explicit BalanceBoard(QObject *parent = nullptr);

	int topLeft() const;
	int topRight() const;
	int bottomLeft() const;
	int bottomRight() const;

private:
	void balanceBoardDataChanged(uint id, int top_left, int top_right, int bottom_left, int bottom_right);

private:
	int m_topLeft = 0;
	int m_topRight = 0;
	int m_bottomLeft = 0;
	int m_bottomRight = 0;

signals:
	void topLeftChanged(int);
	void topRightChanged(int);
	void bottomLeftChanged(int);
	void bottomRightChanged(int);
};
