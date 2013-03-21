#ifndef __STANDARDPRIMITIVE_H__
#define __STANDARDPRIMITIVE_H__

#include <QtCore>
#include "utils.h"
#include "standardshape.h"

// Base class of "standard primitive":
// Butterfly, Circle, Contour, Diamond, Donut,
// Ellipse, Hexagon, Moire, Octagon, Oval,
// RectCenter, RectCham, RectRound, Thermal, Triangle
class StandardPrimitive : public StandardShape
{
public:
  virtual void initialize(QXmlStreamReader& xml) = 0;
};

#endif