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
#ifndef INTEGRATORMODEL_H
#define INTEGRATORMODEL_H
#include "Models/pendulumsystemmodel.h"
#include <QObject>
#include <QJsonObject>

namespace staticpendulum {

/// QML type to manage integrating pendulum system.
class IntegratorModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(double startingStepSize READ startingStepSize WRITE
                 setStartingStepSize NOTIFY startingStepSizeChanged)
  Q_PROPERTY(double maximumStepSize READ maximumStepSize WRITE
                 setMaximumStepSize NOTIFY maximumStepSizeChanged)
  Q_PROPERTY(double relativeTolerance READ relativeTolerance WRITE
                 setRelativeTolerance NOTIFY relativeToleranceChanged)
  Q_PROPERTY(double absoluteTolerance READ absoluteTolerance WRITE
                 setAbsoluteTolerance NOTIFY absoluteToleranceChanged)
  Q_PROPERTY(int threadCount READ threadCount WRITE setThreadCount NOTIFY
                 threadCountChanged)
public:
  explicit IntegratorModel(QObject *parent = 0);

  double startingStepSize() const;
  double maximumStepSize() const;
  double relativeTolerance() const;
  double absoluteTolerance() const;
  int threadCount() const;

  void setStartingStepSize(double startingStepSize);
  void setMaximumStepSize(double maximumStepSize);
  void setRelativeTolerance(double relativeTolerance);
  void setAbsoluteTolerance(double absoluteTolerance);
  void setThreadCount(int threadCount);

  void read(const QJsonObject &json);
  void write(QJsonObject &json) const;

signals:
  void startingStepSizeChanged(double startingStepSize);
  void maximumStepSizeChanged(double maximumStepSize);
  void relativeToleranceChanged(double relativeTolerance);
  void absoluteToleranceChanged(double absoluteTolerance);
  void threadCountChanged(int threadCount);

private:
  double m_startingStepSize;
  double m_maximumStepSize;
  double m_relativeTolerance;
  double m_absoluteTolerance;
  int m_threadCount;
};
} // namespace staticpendulum
#endif // INTEGRATORMODEL_H
