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
#ifndef ATTRACTORLISTMODEL_H
#define ATTRACTORLISTMODEL_H
#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <memory>
#include <vector>

class QJsonArray;
class QJsonValue;

namespace staticpendulum {
struct AttractorModel {
public:
  AttractorModel(double x, double y, double forceCoeff, QColor color);

  double xPosition;
  double yPosition;
  double forceCoefficient;
  QColor color;

  static const QString &xPositionJsonKey();
  static const QString &yPositionJsonKey();
  static const QString &forceCoefficientJsonKey();
  static const QString &colorJsonKey();
};

/// Bindable QObject class that represents a list of attractors.
class AttractorListModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum AttractorRoles {
    xPositionRole = Qt::UserRole,
    yPositionRole,
    forceCoefficientRole,
    colorRole
  };

  explicit AttractorListModel(QObject *parent = 0);

  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;

  auto begin() const { return m_attractors.cbegin(); }
  auto end() const { return m_attractors.cend(); }

  void read(const QJsonArray &jsonArray);
  void write(QJsonArray &jsonArray) const;

public slots:
  void addAttractor(double xPosition, double yPosition, double forceCoeff,
                    QColor color);

  void removeAttractor(int index);

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  std::vector<AttractorModel> m_attractors;
  bool rowExists(int index) const;
};
} // namespace staticpendulum
#endif // ATTRACTORLISTMODEL_H
