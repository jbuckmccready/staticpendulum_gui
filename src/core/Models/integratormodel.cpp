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
#include "integratormodel.h"
#include "DataStorage/jsonreader.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace staticpendulum {
IntegratorModel::IntegratorModel(QObject *parent)
    : QObject(parent), m_startingStepSize(0.001), m_maximumStepSize(0.1),
      m_relativeTolerance(1e-6), m_absoluteTolerance(1e-6), m_threadCount(8) {}

const QString &IntegratorModel::modelJsonKey() {
  static const QString key("integrator");
  return key;
}

const QString &IntegratorModel::startingStepSizeJsonKey() {
  static const QString key("startingStepSize");
  return key;
}

const QString &IntegratorModel::maximumStepSizeJsonKey() {
  static const QString key("maximumStepSize");
  return key;
}

const QString &IntegratorModel::relativeToleranceJsonKey() {
  static const QString key("relativeTolerance");
  return key;
}

const QString &IntegratorModel::absoluteToleranceJsonKey() {
  static const QString key("absoluteTolerance");
  return key;
}

const QString &IntegratorModel::threadCountJsonKey() {
  static const QString key("threadCount");
  return key;
}

double IntegratorModel::startingStepSize() const { return m_startingStepSize; }

double IntegratorModel::maximumStepSize() const { return m_maximumStepSize; }

double IntegratorModel::relativeTolerance() const {
  return m_relativeTolerance;
}

double IntegratorModel::absoluteTolerance() const {
  return m_absoluteTolerance;
}

int IntegratorModel::threadCount() const { return m_threadCount; }

void IntegratorModel::setMaximumStepSize(double maximumStepSize) {
  if (m_maximumStepSize == maximumStepSize)
    return;

  m_maximumStepSize = maximumStepSize;
  emit maximumStepSizeChanged(maximumStepSize);
}

void IntegratorModel::setRelativeTolerance(double relativeTolerance) {
  if (m_relativeTolerance == relativeTolerance)
    return;

  m_relativeTolerance = relativeTolerance;
  emit relativeToleranceChanged(relativeTolerance);
}

void IntegratorModel::setAbsoluteTolerance(double absoluteTolerance) {
  if (m_absoluteTolerance == absoluteTolerance)
    return;

  m_absoluteTolerance = absoluteTolerance;
  emit absoluteToleranceChanged(absoluteTolerance);
}

void IntegratorModel::setThreadCount(int threadCount) {
  if (m_threadCount == threadCount)
    return;

  m_threadCount = threadCount;
  emit threadCountChanged(threadCount);
}

void IntegratorModel::read(const QJsonObject &json) {
  JsonReader reader(modelJsonKey(), json);

  setStartingStepSize(
      reader.readProperty(startingStepSizeJsonKey()).toDouble());

  setMaximumStepSize(reader.readProperty(maximumStepSizeJsonKey()).toDouble());

  setRelativeTolerance(
      reader.readProperty(relativeToleranceJsonKey()).toDouble());

  setAbsoluteTolerance(
      reader.readProperty(absoluteToleranceJsonKey()).toDouble());

  setThreadCount(reader.readProperty(threadCountJsonKey()).toInt());
}

void IntegratorModel::write(QJsonObject &json) const {
  json[startingStepSizeJsonKey()] = m_startingStepSize;
  json[maximumStepSizeJsonKey()] = m_maximumStepSize;
  json[relativeToleranceJsonKey()] = m_relativeTolerance;
  json[absoluteToleranceJsonKey()] = m_absoluteTolerance;
  json[threadCountJsonKey()] = m_threadCount;
}

void IntegratorModel::setStartingStepSize(double startingStepSize) {
  if (m_startingStepSize == startingStepSize)
    return;

  m_startingStepSize = startingStepSize;
  emit startingStepSizeChanged(startingStepSize);
}
} // namespace staticpendulum
