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
#ifndef PENDULUMSYSTEMMODEL_H
#define PENDULUMSYSTEMMODEL_H
#include "attractorlistmodel.h"
#include "CoreEngine/pendulumsystem.h"
#include <QObject>
#include <memory>

/// Bindable QObject class to represent an entire static pendulum system.
class PendulumSystemModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(AttractorListModel *attractorList READ attractorList CONSTANT)
  Q_PROPERTY(
      double distance READ distance WRITE setDistance NOTIFY distanceChanged)
  Q_PROPERTY(double mass READ mass WRITE setMass NOTIFY massChanged)
  Q_PROPERTY(double gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
  Q_PROPERTY(double drag READ drag WRITE setDrag NOTIFY dragChanged)
  Q_PROPERTY(double length READ length WRITE setLength NOTIFY lengthChanged)
public:
  explicit PendulumSystemModel(QObject *parent = 0);

  AttractorListModel *attractorList();
  double distance() const;
  double mass() const;
  double gravity() const;
  double drag() const;
  double length() const;

  void setDistance(double distance);
  void setMass(double mass);
  void setGravity(double gravity);
  void setDrag(double drag);
  void setLength(double length);

  PendulumSystem wrappedSystem() const;

signals:
  void distanceChanged(double distance);
  void massChanged(double mass);
  void gravityChanged(double gravity);
  void dragChanged(double drag);
  void lengthChanged(double length);

private:
  AttractorListModel m_attractorList;
  PendulumSystem m_pendulumSystem;
};

#endif // PENDULUMSYSTEMMODEL_H
