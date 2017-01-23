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
#include "pendulummapmodel.h"
#include "DataStorage/jsonreader.h"
#include <QJsonObject>
#include <QJsonValue>

namespace staticpendulum {
PendulumMapModel::PendulumMapModel(QObject *parent)
    : QObject(parent), m_xStart(-10.0), m_yStart(-10.0), m_xEnd(10.0),
      m_yEnd(10.0), m_resolution(0.05), m_attractorPosThreshold(0.5),
      m_midPosThreshold(0.1), m_convergeTimeThreshold(5.0),
      m_midConvergeColor(QColor(0, 0, 0)),
      m_outOfBoundsColor(QColor(255, 255, 255)) {}

const QString &PendulumMapModel::modelJsonKey()
{
  static const QString key("pendulumMap");
  return key;
}

const QString &PendulumMapModel::xStartJsonKey() {
  static const QString key("xStart");
  return key;
}

const QString &PendulumMapModel::yStartJsonKey() {
  static const QString key("yStart");
  return key;
}

const QString &PendulumMapModel::xEndJsonKey() {
  static const QString key("xEnd");
  return key;
}

const QString &PendulumMapModel::yEndJsonKey() {
  static const QString key("yEnd");
  return key;
}

const QString &PendulumMapModel::resolutionJsonKey() {
  static const QString key("resolution");
  return key;
}

const QString &PendulumMapModel::attractorPosThresholdJsonKey() {
  static const QString key("attractorPosThreshold");
  return key;
}

const QString &PendulumMapModel::midPosThresholdJsonKey() {
  static const QString key("midPosThreshold");
  return key;
}

const QString &PendulumMapModel::convergeTimeThresholdJsonKey() {
  static const QString key("convergeTimeThreshold");
  return key;
}

const QString &PendulumMapModel::midConvergeColorJsonKey() {
  static const QString key("midConvergeColor");
  return key;
}

const QString &PendulumMapModel::outOfBoundsColorJsonKey() {
  static const QString key("outOfBoundsColor");
  return key;
}

double PendulumMapModel::xStart() const { return m_xStart; }

double PendulumMapModel::yStart() const { return m_yStart; }

double PendulumMapModel::xEnd() const { return m_xEnd; }

double PendulumMapModel::yEnd() const { return m_yEnd; }

double PendulumMapModel::resolution() const { return m_resolution; }

double PendulumMapModel::attractorPosThreshold() const {
  return m_attractorPosThreshold;
}

double PendulumMapModel::midPosThreshold() const { return m_midPosThreshold; }

double PendulumMapModel::convergeTimeThreshold() const {
  return m_convergeTimeThreshold;
}

QColor PendulumMapModel::midConvergeColor() const { return m_midConvergeColor; }

QColor PendulumMapModel::outOfBoundsColor() const { return m_outOfBoundsColor; }

void PendulumMapModel::setXStart(double xStart) {
  if (m_xStart == xStart)
    return;

  m_xStart = xStart;
  emit xStartChanged(xStart);
}

void PendulumMapModel::setYStart(double yStart) {
  if (m_yStart == yStart)
    return;

  m_yStart = yStart;
  emit yStartChanged(yStart);
}

void PendulumMapModel::setXEnd(double xEnd) {
  if (m_xEnd == xEnd)
    return;

  m_xEnd = xEnd;
  emit xEndChanged(xEnd);
}

void PendulumMapModel::setYEnd(double yEnd) {
  if (m_yEnd == yEnd)
    return;

  m_yEnd = yEnd;
  emit yEndChanged(yEnd);
}

void PendulumMapModel::setResolution(double resolution) {
  if (m_resolution == resolution)
    return;

  m_resolution = resolution;
  emit resolutionChanged(resolution);
}

void PendulumMapModel::setAttractorPosThreshold(double attractorPositionTol) {
  if (m_attractorPosThreshold == attractorPositionTol)
    return;

  m_attractorPosThreshold = attractorPositionTol;
  emit attractorPosThresholdChanged(attractorPositionTol);
}

void PendulumMapModel::setMidPosThreshold(double midPositionTol) {
  if (m_midPosThreshold == midPositionTol)
    return;

  m_midPosThreshold = midPositionTol;
  emit midPosThresholdChanged(midPositionTol);
}

void PendulumMapModel::setConvergeTimeThreshold(double convergeTimeThreshold) {
  if (m_convergeTimeThreshold == convergeTimeThreshold)
    return;

  m_convergeTimeThreshold = convergeTimeThreshold;
  emit convergeTimeThresholdChanged(convergeTimeThreshold);
}

void PendulumMapModel::setMidConvergeColor(QColor midConvergeColor) {
  if (m_midConvergeColor == midConvergeColor)
    return;

  m_midConvergeColor = midConvergeColor;
  emit midConvergeColorChanged(midConvergeColor);
}

void PendulumMapModel::setOutOfBoundsColor(QColor outOfBoundsColor) {
  if (m_outOfBoundsColor == outOfBoundsColor)
    return;

  m_outOfBoundsColor = outOfBoundsColor;
  emit outOfBoundsColorChanged(outOfBoundsColor);
}

void PendulumMapModel::read(const QJsonObject &json) {
  const JsonReader reader("pendulumMap", json);
  setXStart(reader.readProperty(xStartJsonKey()).toDouble());
  setYStart(reader.readProperty(yStartJsonKey()).toDouble());
  setXEnd(reader.readProperty(xEndJsonKey()).toDouble());
  setYEnd(reader.readProperty(yEndJsonKey()).toDouble());
  setResolution(reader.readProperty(resolutionJsonKey()).toDouble());
  setMidPosThreshold(reader.readProperty(midPosThresholdJsonKey()).toDouble());
  setConvergeTimeThreshold(
      reader.readProperty(convergeTimeThresholdJsonKey()).toDouble());
  setMidConvergeColor(reader.readPropertyAsQColor(midConvergeColorJsonKey()));
  setOutOfBoundsColor(reader.readPropertyAsQColor(outOfBoundsColorJsonKey()));
}

void PendulumMapModel::write(QJsonObject &json) const {
  json[xStartJsonKey()] = xStart();
  json[yStartJsonKey()] = yStart();
  json[xEndJsonKey()] = xEnd();
  json[yEndJsonKey()] = yEnd();
  json[resolutionJsonKey()] = resolution();
  json[midPosThresholdJsonKey()] = midPosThreshold();
  json[convergeTimeThresholdJsonKey()] = convergeTimeThreshold();
  json[midConvergeColorJsonKey()] = midConvergeColor().name();
  json[outOfBoundsColorJsonKey()] = outOfBoundsColor().name();
}
} // namespace staticpendulum
