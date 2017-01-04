#ifndef JSONREADER_H
#define JSONREADER_H
#include <QColor>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace staticpendulum {

/// Class to read Json values and log when they are not found or do not match
/// expected types.
class JsonReader {
public:
  /// Construct a reader with a parent name to use for logging when reading
  /// properties.
  JsonReader(QString parentName, QJsonObject json);

  QJsonValue
  readProperty(const QString &propName,
               QJsonValue::Type expectedType = QJsonValue::Type::Double) const;

  /// Attempts to read the json property as a QColor object. If the property is
  /// not a string or the property as a string is not a valid color name returns
  /// black otherwise returns the color name parsed from the property as a string.
  QColor readPropertyAsQColor(const QString &propName);

private:
  /// Logs that the property was missing when attempting to read.
  void logMissingProp(const QString &propName) const;

  /// Logs that the property type found does not match the expected type.
  void logWrongType(const QString &propName, QJsonValue::Type expected,
                    QJsonValue::Type found) const;

  /// Converts QJsonValue::Type enum to QString.
  QString jsonValueTypeToString(QJsonValue::Type jsonType) const;

  QString m_parentName;
  QJsonObject m_json;
};
} // namespace staticpendulum
#endif // JSONREADER_H
