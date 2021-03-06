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
#include "pendulumsystemmodel.h"
#include "DataStorage/jsonreader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <cmath>

namespace staticpendulum {
PendulumSystemModel::PendulumSystemModel(QObject *parent) : QObject(parent) {
  m_pendulumSystem.distance = 0.05;
  m_pendulumSystem.mass = 1.0;
  m_pendulumSystem.gravity = 9.8;
  m_pendulumSystem.drag = 0.2;
  m_pendulumSystem.length = 10.0;
  // Create default ring of attractors
  const double yMag = std::sqrt(1 - 0.5 * 0.5);
  m_attractors.addAttractor(-0.5, yMag, 1, QColor(255, 140, 0));
  m_attractors.addAttractor(-0.5, -yMag, 1, QColor(30, 144, 255));
  m_attractors.addAttractor(1.0, 0.0, 1, QColor(178, 34, 34));
  //  for (int i = 0; i < 1000; ++i) {
  //    m_attractors.addAttractor(1, 2, 3, QColor("orange"));
  //  }
}

const QString &PendulumSystemModel::modelJsonKey() {
  static const QString key("pendulumSystem");
  return key;
}

const QString &PendulumSystemModel::distanceJsonKey() {
  static const QString key("distance");
  return key;
}

const QString &PendulumSystemModel::massJsonKey() {
  static const QString key("mass");
  return key;
}

const QString &PendulumSystemModel::gravityJsonKey() {
  static const QString key("gravity");
  return key;
}

const QString &PendulumSystemModel::dragJsonKey() {
  static const QString key("drag");
  return key;
}

const QString &PendulumSystemModel::lengthJsonKey() {
  static const QString key("length");
  return key;
}

const QString &PendulumSystemModel::attractorsJsonKey() {
  static const QString key("attractors");
  return key;
}

AttractorListModel *PendulumSystemModel::attractors() { return &m_attractors; }

double PendulumSystemModel::distance() const {
  return m_pendulumSystem.distance;
}

double PendulumSystemModel::mass() const { return m_pendulumSystem.mass; }

double PendulumSystemModel::gravity() const { return m_pendulumSystem.gravity; }

double PendulumSystemModel::drag() const { return m_pendulumSystem.drag; }

double PendulumSystemModel::length() const { return m_pendulumSystem.length; }

PendulumSystem PendulumSystemModel::wrappedSystem() const {
  PendulumSystem result = m_pendulumSystem;
  result.attractorList.reserve(m_attractors.rowCount());

  auto end = m_attractors.end();
  for (auto iter = m_attractors.begin(); iter != end; ++iter) {
    result.attractorList.emplace_back(iter->xPosition, iter->yPosition,
                                      iter->forceCoefficient);
  }

  return result;
}

void PendulumSystemModel::setDistance(double distance) {
  if (m_pendulumSystem.distance == distance)
    return;

  m_pendulumSystem.distance = distance;
  emit distanceChanged(distance);
}

void PendulumSystemModel::setMass(double mass) {
  if (m_pendulumSystem.mass == mass)
    return;

  m_pendulumSystem.mass = mass;
  emit massChanged(mass);
}

void PendulumSystemModel::setGravity(double gravity) {
  if (m_pendulumSystem.gravity == gravity)
    return;

  m_pendulumSystem.gravity = gravity;
  emit gravityChanged(gravity);
}

void PendulumSystemModel::setDrag(double drag) {
  if (m_pendulumSystem.drag == drag)
    return;

  m_pendulumSystem.drag = drag;
  emit dragChanged(drag);
}

void PendulumSystemModel::setLength(double length) {
  if (m_pendulumSystem.length == length)
    return;

  m_pendulumSystem.length = length;
  emit lengthChanged(length);
}

void PendulumSystemModel::read(const QJsonObject &json) {
  const JsonReader reader("pendulumSystem", json);
  setDistance(reader.readProperty(distanceJsonKey()).toDouble());
  setMass(reader.readProperty(massJsonKey()).toDouble());
  setGravity(reader.readProperty(gravityJsonKey()).toDouble());
  setDrag(reader.readProperty(dragJsonKey()).toDouble());
  setLength(reader.readProperty(dragJsonKey()).toDouble());

  const QJsonValue attractorsJson =
      reader.readProperty(attractorsJsonKey(), QJsonValue::Type::Array);
  if (attractorsJson.type() == QJsonValue::Type::Null) {
    return;
  }

  m_attractors.read(attractorsJson.toArray());
}

void PendulumSystemModel::write(QJsonObject &json) const {
  json[distanceJsonKey()] = distance();
  json[massJsonKey()] = mass();
  json[gravityJsonKey()] = gravity();
  json[dragJsonKey()] = drag();
  json[lengthJsonKey()] = length();
  QJsonArray attractorsJsonArr = QJsonArray();
  m_attractors.write(attractorsJsonArr);
  json[attractorsJsonKey()] = attractorsJsonArr;
}
} // namespace staticpendulum
