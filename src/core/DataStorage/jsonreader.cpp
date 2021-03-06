#include "jsonreader.h"
#include <QDebug>

namespace staticpendulum {
JsonReader::JsonReader(QString parentName, QJsonObject json)
    : m_parentName(parentName), m_json(json) {}

void JsonReader::logMissingProp(const QString &propName) const {
  qCritical() << QString("Missing %1 parameter in JSON: %2.")
                     .arg(m_parentName, propName);
}

void JsonReader::logWrongType(const QString &propName,
                              QJsonValue::Type expected,
                              QJsonValue::Type found) const {
  qCritical() << QString("Wrong type for %1 parameter in JSON: %2. Expected %3 "
                         "but found %4.")
                     .arg(m_parentName, propName,
                          jsonValueTypeToString(expected),
                          jsonValueTypeToString(found));
}

QString JsonReader::jsonValueTypeToString(QJsonValue::Type jsonType) const {
  switch (jsonType) {
  case QJsonValue::Type::Array:
    return QString("Array");
  case QJsonValue::Type::Bool:
    return QString("Bool");
  case QJsonValue::Type::Double:
    return QString("Double");
  case QJsonValue::Type::Null:
    return QString("Null");
  case QJsonValue::Type::Object:
    return QString("Object");
  case QJsonValue::Type::String:
    return QString("String");
  case QJsonValue::Type::Undefined:
    return QString("Undefined");
  }

  Q_UNREACHABLE();
}

QJsonValue JsonReader::readProperty(const QString &propName,
                                    QJsonValue::Type expectedType) const {

  if (!m_json.contains(propName)) {
    logMissingProp(propName);
    return QJsonValue();
  }

  QJsonValue result = m_json[propName];

  if (result.type() != expectedType) {
    logWrongType(propName, expectedType, result.type());
    return QJsonValue();
  }

  return result;
}

QColor JsonReader::readPropertyAsQColor(const QString &propName) const {

  QJsonValue result = readProperty(propName, QJsonValue::Type::String);
  if (result.type() == QJsonValue::Type::Null) {
    return QColor("black");
  }

  QString colorString(readProperty(propName, QJsonValue::Type::String).toString());
  if (!QColor::isValidColor(colorString)) {
    return QColor("black");
  }

  return QColor(colorString);
}
} // namespace staticpendulum
