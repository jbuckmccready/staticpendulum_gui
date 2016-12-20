/* ===========================================================================
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jedidiah Buck McCready <jbuckmccready@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ===========================================================================*/
#include "Models/rootmodel.h"
#include "QmlHelpers/systemintegrator.h"
#include <QApplication>
#include <QHash>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  using namespace staticpendulum;

  qmlRegisterType<SystemIntegrator>("QmlHelpers", 1, 0, "SystemIntegrator");

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("applicationDirPath",
                                           qApp->applicationDirPath());

  auto *rootModel = new RootModel();
  engine.rootContext()->setContextProperty("pendulumSystemModel",
                                           rootModel->pendulumSystemModel());
  engine.rootContext()->setContextProperty("integratorModel",
                                           rootModel->integratorModel());
  engine.rootContext()->setContextProperty("pendulumMapModel",
                                           rootModel->pendulumMapModel());
  engine.load(QUrl(QLatin1String("qrc:/qml/Main.qml")));

  return app.exec();
}
