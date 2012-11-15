#ifndef VIRTUALCURSOR_H
#define VIRTUALCURSOR_H

#include <QList>
#include <QPointF>
#include "adaptors/deviceevents.h"

#include <qmath.h>

class VirtualCursor
{
private:
  QPointF m_cursor;
  bool m_visible;

  double m_angle[2];
  int m_index;

  qint16 m_ctable[3][2];
  qint16 m_ltable[3][2];

private:
  double angleDiff(double a, double b);

public:
  VirtualCursor();

  bool calculate(QList < struct irpoint > &points, double roll);
  bool visible();
  QPointF& cursor();
  double angle();

};

#endif // VIRTUALCURSOR_H
