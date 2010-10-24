#ifndef WIIMOTELEDITEM_H
#define WIIMOTELEDITEM_H

#include <QGraphicsPixmapItem>

class WiimoteLedItem : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
private:
  bool status;

public:
  WiimoteLedItem(QObject *parent = 0);

protected:
  virtual void mousePressEvent (QGraphicsSceneMouseEvent*);

public Q_SLOTS:
  void switchOn();
  void switchOff();

Q_SIGNALS:
  void ledSwitched(bool);

};

#endif // WIIMOTELEDITEM_H
