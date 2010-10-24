#include "wiimoteleditem.h"
#include <QGraphicsSceneMouseEvent>

WiimoteLedItem::WiimoteLedItem(QObject *parent) :
  QObject(parent),
  QGraphicsPixmapItem()
{
  switchOff();
}

void WiimoteLedItem::switchOn() {
  setPixmap(QPixmap(":/led_on.png"));
  status = true;
}

void WiimoteLedItem::switchOff() {
  setPixmap(QPixmap(":/led_off.png"));
  status = false;
}

void WiimoteLedItem::mousePressEvent (QGraphicsSceneMouseEvent *event) {
  if (status)
    switchOff(); else
    switchOn();

  emit ledSwitched(status);
}
