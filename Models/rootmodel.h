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
#ifndef ROOTMODEL_H
#define ROOTMODEL_H
#include "integratormodel.h"
#include "pendulummapmodel.h"
#include "pendulumsystemmodel.h"
#include <QObject>

/// Class to hold all the models and handle loading/saving models.
class RootModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(PendulumSystemModel *pendulumSystemModel READ pendulumSystemModel
                 CONSTANT)
  Q_PROPERTY(PendulumMapModel *pendulumMapModel READ pendulumMapModel CONSTANT)
  Q_PROPERTY(IntegratorModel *integratorModel READ integratorModel CONSTANT)

public:
  explicit RootModel(QObject *parent = 0);

  PendulumSystemModel *pendulumSystemModel();
  PendulumMapModel *pendulumMapModel();
  IntegratorModel *integratorModel();

signals:

public slots:

private:
  PendulumSystemModel m_pendulumSystemModel;
  IntegratorModel m_integratorModel;
  PendulumMapModel m_pendulumMapModel;
};

#endif // ROOTMODEL_H
