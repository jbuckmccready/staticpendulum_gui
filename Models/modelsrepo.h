#ifndef MODELSREPO_H
#define MODELSREPO_H

#include "integratormodel.h"
#include "pendulummapmodel.h"
#include "pendulumsystemmodel.h"
#include <QObject>

class QQmlEngine;
class QJSEngine;

namespace staticpendulum {
class ModelsRepo : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(ModelsRepo)

  Q_PROPERTY(PendulumSystemModel *pendulumSystemModel READ pendulumSystemModel
                 CONSTANT)
  Q_PROPERTY(PendulumMapModel *pendulumMapModel READ pendulumMapModel CONSTANT)
  Q_PROPERTY(IntegratorModel *integratorModel READ integratorModel CONSTANT)

public:
  PendulumSystemModel *pendulumSystemModel();
  PendulumMapModel *pendulumMapModel();
  IntegratorModel *integratorModel();

  static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

private:
  ModelsRepo();
  PendulumSystemModel m_pendulumSystemModel;
  IntegratorModel m_integratorModel;
  PendulumMapModel m_pendulumMapModel;
};
} // namespace staticpendulum
#endif // MODELSREPO_H
