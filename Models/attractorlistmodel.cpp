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
#include "attractorlistmodel.h"

// **********************************************************************
// Attractor impelementation
// **********************************************************************
Attractor::Attractor(double x, double y, double forceCoeff, QColor color)
    : xPosition{x}, yPosition{y}, forceCoefficient{forceCoeff}, color{color} {}

// **********************************************************************
// AttractorListModel impelementation
// **********************************************************************
AttractorListModel::AttractorListModel(QObject *parent)
    : QAbstractListModel(parent) {}

void AttractorListModel::addAttractor(double xPosition, double yPosition,
                                      double forceCoeff, QColor color) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_attractors.emplace_back(xPosition, yPosition, forceCoeff, color);
  endInsertRows();
}

bool AttractorListModel::removeRows(int row, int count,
                                    const QModelIndex &parent) {
  Q_UNUSED(parent);
  int endRow = row + count - 1;
  if (!rowExists(row) || !rowExists(endRow))
    return false;
  beginRemoveRows(QModelIndex(), row, endRow);
  // stl iterator uses one past the end index to designate end
  m_attractors.erase(m_attractors.begin() + row,
                     m_attractors.begin() + endRow + 1);
  endRemoveRows();
  return true;
}

void AttractorListModel::removeAttractor(int index) {
  removeRows(index, 1, QModelIndex());
}

int AttractorListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return static_cast<int>(m_attractors.size());
}

QVariant AttractorListModel::data(const QModelIndex &index, int role) const {
  if (!rowExists(index.row()))
    return QVariant();

  const Attractor &attractor = m_attractors[index.row()];
  switch (role) {
  case xPositionRole:
    return attractor.xPosition;
  case yPositionRole:
    return attractor.yPosition;
  case forceCoefficientRole:
    return attractor.forceCoefficient;
  case colorRole:
    return attractor.color;
  default:
    return QVariant();
  }
}

bool AttractorListModel::setData(const QModelIndex &index,
                                 const QVariant &value, int role) {

  if (!rowExists(index.row()))
    return false;

  Attractor &attractor = m_attractors[index.row()];
  bool valueChanged = false;

  switch (role) {
  case xPositionRole:
    if ((valueChanged = attractor.xPosition != value))
      attractor.xPosition = value.toDouble();
    break;
  case yPositionRole:
    if ((valueChanged = attractor.yPosition != value))
      attractor.yPosition = value.toDouble();
    break;
  case forceCoefficientRole:
    if ((valueChanged = attractor.forceCoefficient != value))
      attractor.forceCoefficient = value.toDouble();
    break;
  case colorRole:
    if ((valueChanged = attractor.color != value))
      attractor.color = value.value<QColor>();
    break;
  default:
    return false;
  }
  if (valueChanged) {
    QVector<int> changedRoles = {role};
    emit dataChanged(index, index, changedRoles);
  }

  return true;
}

QHash<int, QByteArray> AttractorListModel::roleNames() const {
  QHash<int, QByteArray> roles{{xPositionRole, "xPosition"},
                               {yPositionRole, "yPosition"},
                               {forceCoefficientRole, "forceCoefficient"},
                               {colorRole, "color"}};
  return roles;
}

bool AttractorListModel::rowExists(int rowIndex) const {
  return rowIndex >= 0 && rowIndex < rowCount();
}
