#include "modelsrepo.h"
#include "DataStorage/jsonreader.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace staticpendulum {
ModelsRepo::ModelsRepo() : QObject(nullptr) {}
PendulumSystemModel *ModelsRepo::pendulumSystemModel() {
  return &m_pendulumSystemModel;
}

PendulumMapModel *ModelsRepo::pendulumMapModel() { return &m_pendulumMapModel; }

IntegratorModel *ModelsRepo::integratorModel() { return &m_integratorModel; }

QObject *ModelsRepo::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);

  return new ModelsRepo();
}

void ModelsRepo::loadJsonFile(const QString &filePath) {
  QFile jsonFile(filePath);
  if (!jsonFile.open(QIODevice::ReadOnly)) {
    qCritical() << QString("Could not open file: %1. Error: %2")
                       .arg(filePath, jsonFile.errorString());
    return;
  }

  QByteArray fileData = jsonFile.readAll();
  QJsonParseError parseError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);
  if (parseError.error != QJsonParseError::NoError) {
    qCritical() << QString("Parsing json failed. Error: %1. Offset: %2")
                       .arg(parseError.errorString(), parseError.offset);
    return;
  }

  QJsonObject rootObj = jsonDoc.object();

  JsonReader reader("root", rootObj);

  QJsonObject pendulumSystemObj =
      reader
          .readProperty(PendulumSystemModel::modelJsonKey(),
                        QJsonValue::Type::Object)
          .toObject();

  m_pendulumSystemModel.read(pendulumSystemObj);

  QJsonObject integratorObj = reader
                                  .readProperty(IntegratorModel::modelJsonKey(),
                                                QJsonValue::Type::Object)
                                  .toObject();
  m_integratorModel.read(integratorObj);

  QJsonObject pendulumMapObj =
      reader
          .readProperty(PendulumMapModel::modelJsonKey(),
                        QJsonValue::Type::Object)
          .toObject();
  m_pendulumMapModel.read(pendulumMapObj);
}

void ModelsRepo::saveJsonFile(const QString &filePath) {
  QFile jsonFile(filePath);
  if (!jsonFile.open(QIODevice::WriteOnly)) {
    qCritical() << QString("Could not write to file location: %1. Error: %2")
                       .arg(filePath, jsonFile.errorString());
    return;
  }

  QJsonObject rootObject;

  QJsonObject pendulumSystemObj;
  m_pendulumSystemModel.write(pendulumSystemObj);
  rootObject[PendulumSystemModel::modelJsonKey()] = pendulumSystemObj;

  QJsonObject integratorObj;
  m_integratorModel.write(integratorObj);
  rootObject[IntegratorModel::modelJsonKey()] = integratorObj;

  QJsonObject pendulumMapObj;
  m_pendulumMapModel.write(pendulumMapObj);
  rootObject[PendulumMapModel::modelJsonKey()] = pendulumMapObj;

  QJsonDocument jsonDoc(rootObject);
  jsonFile.write(jsonDoc.toJson());
}
} // namespace staticpendulum
