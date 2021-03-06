#include "utils.h"
#include "error.h"

bool isEndElementWithName(const QXmlStreamReader& xml, const QString& name)
{
  return (xml.isEndElement() && xml.name() == name);
}

bool isStartElementWithName(const QXmlStreamReader& xml, const QString& name)
{
  return (xml.isStartElement() && xml.name() == name);
}

const QString getAttributeOrCharacters(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  // first search for attribute
  QString value = getAttribute(xml, attributeName);
  // if failed, search for nested text
  if (value == "") {
    value = xml.readElementText();
  }
  // if still failed, it is an error
  if (value == "") {
    throw new InvalidAttributeError(elementName, attributeName);
  }
  return value;
}

bool hasAttribute(const QXmlStreamReader& xml, const QString attrName)
{
  return xml.attributes().hasAttribute(attrName);
}

const QString getAttribute(const QXmlStreamReader& xml, const QString attrName)
{
  return xml.attributes().value("", attrName).toString();
}

qreal getNonNegativeDoubleAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  qreal value = getDoubleAttribute(xml, elementName, attributeName);
  if (value < 0) {
    throw new InvalidAttributeError(elementName, attributeName);
  }
  return value;
}

qreal getDoubleAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  bool ok;
  qreal value = getAttribute(xml, attributeName).toDouble(&ok);
  if (!ok) {
    throw new InvalidAttributeError(elementName, attributeName);
  }
  return value;
}

int getNonNegativeIntAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  int value = getIntAttribute(xml, elementName, attributeName);
  if (value < 0) {
    throw new InvalidAttributeError(elementName, attributeName);
  }
  return value;
}

int getIntAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  bool ok;
  int value = getAttribute(xml, attributeName).toInt(&ok);
  if (!ok) {
    throw new InvalidAttributeError(elementName, attributeName);
  }
  return value;
}

bool getBoolAttribute(QXmlStreamReader& xml, const QString attributeName)
{
  return (getAttribute(xml, attributeName) == "TRUE");
}

UnitsType getUnitAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  // there are only 3 kinds of unit in ipc-2581
  QString units = getAttribute(xml, attributeName);
  if (units == "MILLIMETER") {
    return MILLIMETER;
  }
  else if (units == "MICRON") {
    return MICRON;
  }
  else if (units == "INCH") {
    return INCH;
  }
  throw new InvalidAttributeError(elementName, attributeName);
}

QString getStringAttribute(QXmlStreamReader& xml,
    const QString elementName, const QString attributeName)
{
  // Note: here doesn't treat empty string "" as error,
  // because in some test cases there are empty strings...don't know why
  return getAttribute(xml, attributeName);
}

qreal toInch(qreal num, UnitsType currentUnits)
{
  switch (currentUnits) {
    case MILLIMETER :
      return num / 25.4f;
    case MICRON :
      return num / 1000 / 25.4f;
    case INCH :
      return num;
    case MIL :
      return num / 1000;
    default :
      break;
  }
  throw new ProgramLogicError("toInch");
}

qreal toMil(qreal num, UnitsType currentUnits)
{
  switch (currentUnits) {
    case MILLIMETER :
      return num * 1000 / 25.4f;
    case MICRON :
      return num / 25.4f;
    case INCH :
      return num * 1000;
    case MIL :
      return num;
    default :
      break;
  }
  throw new ProgramLogicError("toMil");
}

bool isSubstitutionGroupAttribute(QStringRef elementName)
{
  return (elementName == "BooleanAttribute" ||
          elementName == "DoubleAttribute" ||
          elementName == "IntegerAttribute" ||
          elementName == "NonstandardAttribute" ||
          elementName == "OptionAttribute" ||
          elementName == "TextAttribute");
}

bool isSubstitutionGroupColorGroup(QStringRef elementName)
{
  return (elementName == "Color" ||
          elementName == "ColorRef");
}

bool isSubstitutionGroupFeature(QStringRef elementName)
{
  return (isSubstitutionGroupStandardShape(elementName) ||
          isSubstitutionGroupUserShape(elementName));
}

bool isSubstitutionGroupFiducial(QStringRef elementName)
{
  return (elementName == "BadBoardMark" ||
          elementName == "GlobalFiducial" ||
          elementName == "GoodPanelMark" ||
          elementName == "LocalFiducial");
}

bool isSubstitutionGroupLineDescGroup(QStringRef elementName)
{
  return (elementName == "LineDesc" ||
          elementName == "LineDescRef");
}

bool isSubstitutionGroupPolyStep(QStringRef elementName)
{
  return (elementName == "PolyStepCurve" ||
          elementName == "PolyStepSegment");
}

bool isSubstitutionGroupSimple(QStringRef elementName)
{
  return (elementName == "Arc" ||
          elementName == "Line" ||
          elementName == "Outline" ||
          elementName == "Polyline");
}

bool isSubstitutionGroupStandardPrimitive(QStringRef elementName)
{
  return (elementName == "Butterfly" ||
          elementName == "Circle" ||
          elementName == "Contour" ||
          elementName == "Diamond" ||
          elementName == "Donut" ||
          elementName == "Ellipse" ||
          elementName == "Hexagon" ||
          elementName == "Moire" ||
          elementName == "Octagon" ||
          elementName == "Oval" ||
          elementName == "RectCenter" ||
          elementName == "RectCham" ||
          elementName == "RectCorner" ||
          elementName == "RectRound" ||
          elementName == "Thermal" ||
          elementName == "Triangle");
}

bool isSubstitutionGroupStandardShape(QStringRef elementName)
{
  return (isSubstitutionGroupStandardPrimitive(elementName) ||
          elementName == "StandardPrimitiveRef");
}

bool isSubstitutionGroupUserPrimitive(QStringRef elementName)
{
  return (isSubstitutionGroupSimple(elementName) ||
          elementName == "Text" ||
          elementName == "UserSpecial");
}

bool isSubstitutionGroupUserShape(QStringRef elementName)
{
  return (isSubstitutionGroupUserPrimitive(elementName) ||
          elementName == "UserPrimitiveRef");
}

QPointF calcTransformedLocation(QPointF location, Xform *xform)
{
  if (xform == NULL) {
    return location;
  }
  return QPointF(location.x() + xform->offset().x() * xform->scale(),
                 location.y() + xform->offset().y() * xform->scale());
}

int odbDecideOrient(Xform *xform)
{
  // 0 : 0 degrees, no mirror
  // 1 : 90 degrees, no mirror
  // 2 : 180 degrees, no mirror
  // 3 : 270 degrees, no mirror
  // 4 : 0 degrees, mirror in X axis
  // 5 : 90 degrees, mirror in X axis
  // 6 : 180 degrees, mirror in X axis
  // 7 : 270 degrees, mirror in X axis
  if (xform == NULL) {
    return 0;
  }

  qreal degree = xform->rotation();
  bool mirror = xform->mirror();
  if (!mirror) {
    if (degree == 0) {
      return 0;
    }
    else if (degree == 90) {
      return 1;
    }
    else if (degree == 180) {
      return 2;
    }
    else if (degree == 270) {
      return 3;
    }
    else { // not 90 increment
      return 0;
    }
  }
  else {
    if (degree == 0) {
      return 4;
    }
    else if (degree == 90) {
      return 5;
    }
    else if (degree == 180) {
      return 6;
    }
    else if (degree == 270) {
      return 7;
    }
    else { // not 90 increment
      return 4;
    }
  }
}

int odbInsertSymbol(const QString symbol, QList<QString>& symbolsTable)
{
  // insert symbol if necessary, return its final index
  int symNum = symbolsTable.indexOf(symbol);
  if (symNum == -1) {
    symNum = symbolsTable.size();
    symbolsTable.append(symbol);
  }
  return symNum;
}

QString odbRotationSuffix(Xform *xform)
{
  if (xform == NULL) {
    return "";
  }
  qreal degree = xform->rotation();
  if (degree ==   0 ||
      degree ==  90 ||
      degree == 180 ||
      degree == 270) {
    return ""; // 90 increment, no need to suffix (handled by 'orient')
  }
  return QString("_%1").arg(degree);
}

QPointF applyXform(Xform *xform, QPointF src)
{
  if (xform == NULL) {
    return src;
  }

  // modify the origin
  QPointF dst = src + xform->offset();

  // apply rotation
  qreal rotationRad = xform->rotation() / 180 * M_PI;
  qreal ct = qCos(rotationRad);
  qreal st = qSin(rotationRad);
  qreal x = ct * dst.x() - st * dst.y();
  qreal y = st * dst.x() + ct * dst.y();
  dst.setX(x);
  dst.setY(y);

  // mirror image
  if (xform->mirror()) {
    dst.setX(-dst.x());
  }

  // scale
  dst.setX(dst.x() * xform->scale());
  dst.setY(dst.y() * xform->scale());

  // return the result
  return dst;
}

QString saveQreal(qreal num)
{
  QString str = QString::number(num, 'f');
  str.remove(QRegExp("0+$")); // Remove any number of trailing 0's
  str.remove(QRegExp("\\.$")); // If the last character is just a '.', remove it
  return str;
}

qreal calcCorrectAngle(QPointF p0, QPointF p1)
{
  QPointF v = p1 - p0;
  qreal arcTan = qAtan(v.y() / v.x());
  if (v.x() < 0) {
    return arcTan + M_PI;
  }
  else if (v.y() < 0) {
    return arcTan + 2 * M_PI;
  }
  return arcTan;
}

QPointF rotatePoint(QPointF point, qreal degree)
{
  qreal radian = degree / 180 * M_PI;
  qreal x = qCos(radian) * point.x() - qSin(radian) * point.y();
  qreal y = qSin(radian) * point.x() + qCos(radian) * point.y();
  return QPointF(x, y);
}

qreal equalAngle(qreal angle)
{
  while (angle < 0) {
    angle += 360;
  }
  while (angle >= 360) {
    angle -= 360;
  }
  return angle;
}
