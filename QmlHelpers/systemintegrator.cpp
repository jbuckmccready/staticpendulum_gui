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
#include "systemintegrator.h"
#include "CoreEngine/cashkarp54.h"
#include "CoreEngine/pendulummapintegrator.h"
#include <QFutureWatcher>
#include <QImage>
#include <QtConcurrent/QtConcurrent>

SystemIntegrator::SystemIntegrator(QObject *parent) : QObject(parent) {
  QObject::connect(&m_futureWatcher, &QFutureWatcher<void>::finished, this,
                   &createImageFile);
  QObject::connect(&m_futureWatcher,
                   &QFutureWatcher<void>::progressValueChanged, this,
                   &setProgressValue);
  QObject::connect(&m_futureWatcher,
                   &QFutureWatcher<void>::progressRangeChanged, this,
                   [this](int minimum, int maximum) {
                     setProgressMinimum(minimum);
                     setProgressMaximum(maximum);
                   });
}

int SystemIntegrator::progressValue() const { return m_progressValue; }

int SystemIntegrator::progressMinimum() const { return m_progressMinimum; }

int SystemIntegrator::progressMaximum() const { return m_progressMaximum; }

void SystemIntegrator::integrateMap(PendulumSystemModel *pendulumSystemModel,
                                    PendulumMapModel *pendulumMapModel,
                                    IntegratorModel *integratorModel) {
  const auto pendulumSystem = pendulumSystemModel->wrappedSystem();
  const double relTol = integratorModel->relativeTolerance();
  const double absTol = integratorModel->absoluteTolerance();
  const double maxStepSize = integratorModel->maximumStepSize();

  // partially apply the cashKarp54 integrator function
  auto integrator = [=](auto &&dxdt, auto &x, auto &t, auto &h) {
    return cashKarp54(dxdt, x, t, h, relTol, absTol, maxStepSize);
  };

  const double startingStepSize = integratorModel->startingStepSize();
  const double attractorPosThreshold =
      pendulumMapModel->attractorPosThreshold();
  const double midPosThreshold = pendulumMapModel->attractorPosThreshold();
  const double convergeTimeThreshold =
      pendulumMapModel->convergeTimeThreshold();

  // partially apply the integratePoint function
  auto integratePoint = [=](staticpendulum::Point &point) {
    staticpendulum::integratePoint(integrator, pendulumSystem, point,
                                   startingStepSize, attractorPosThreshold,
                                   midPosThreshold, convergeTimeThreshold);
  };

  QThreadPool::globalInstance()->setMaxThreadCount(
      integratorModel->threadCount());

  // set the point map
  m_pointMap = staticpendulum::Map(
      pendulumMapModel->xStart(), pendulumMapModel->yStart(),
      pendulumMapModel->xEnd(), pendulumMapModel->yEnd(),
      pendulumMapModel->resolution());

  // create the color map to be used
  m_colorMap.clear();
  m_colorMap[-2] = pendulumMapModel->outOfBoundsColor();
  m_colorMap[-1] = pendulumMapModel->midConvergeColor();
  int index = 0;
  for (const auto &attractor : *pendulumSystemModel->attractorList()) {
    m_colorMap[index] = attractor.color;
    ++index;
  }

  // start integrating the points
  m_futureWatcher.setFuture(
      QtConcurrent::map(m_pointMap.begin(), m_pointMap.end(), integratePoint));
}

void SystemIntegrator::cancelIntegration() {
  m_futureWatcher.cancel();
  m_futureWatcher.waitForFinished();
}

void SystemIntegrator::createImageFile() {
  const auto rows = m_pointMap.rows();
  const auto cols = m_pointMap.cols();
  QImage image(cols, rows, QImage::Format_RGB32);

  for (std::size_t y = 0; y < rows; ++y) {
    for (std::size_t x = 0; x < cols; ++x) {
      // data is row major oriented
      int index = y * cols + x;
      const int convergePosition =
          (m_pointMap.begin() + index)->convergePosition;
      const QColor color = m_colorMap[convergePosition];
      image.setPixelColor(x, y, color);
    }
  }

  image.save(qApp->applicationDirPath() + "/last_integrated.png");

  emit finishedIntegration();
}

void SystemIntegrator::setProgressValue(int progressValue) {
  if (m_progressValue == progressValue)
    return;

  m_progressValue = progressValue;
  emit progressValueChanged(progressValue);
}

void SystemIntegrator::setProgressMinimum(int progressMinimum) {
  if (m_progressMinimum == progressMinimum)
    return;

  m_progressMinimum = progressMinimum;
  emit progressMinimumChanged(progressMinimum);
}

void SystemIntegrator::setProgressMaximum(int progressMaximum) {
  if (m_progressMaximum == progressMaximum)
    return;

  m_progressMaximum = progressMaximum;
  emit progressMaximumChanged(progressMaximum);
}
