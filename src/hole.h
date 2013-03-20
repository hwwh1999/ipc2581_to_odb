#ifndef __HOLE_H__
#define __HOLE_H__

#include <QtCore>

class Hole
{
public:
  void initialize(QXmlStreamReader& xml);
  enum PlatingStatusType { PLATED, NONPLATED, VIA };

private:
  QString m_name;
  qreal m_diameter;
  PlatingStatusType m_platingStatus;
  qreal m_plusTol;
  qreal m_minusTol;
  QPointF m_location; // (x,y)
};

#endif
