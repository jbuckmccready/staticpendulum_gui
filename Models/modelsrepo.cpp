#include "modelsrepo.h"
namespace staticpendulum {
ModelsRepo::ModelsRepo() : QObject(nullptr)
{

}
PendulumSystemModel *ModelsRepo::pendulumSystemModel() {
  return &m_pendulumSystemModel;
}

IntegratorModel *ModelsRepo::integratorModel() { return &m_integratorModel; }

QObject *ModelsRepo::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);

  return new ModelsRepo();
}

PendulumMapModel *ModelsRepo::pendulumMapModel() { return &m_pendulumMapModel; }
} // namespace staticpendulum
