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
#ifndef PENDULUMMAPMODEL_H
#define PENDULUMMAPMODEL_H
#include <QColor>
#include <QObject>

namespace staticpendulum {
/// Model for the XY map properties that describe the points to integrate
/// across and determine convergence.
class PendulumMapModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(double xStart READ xStart WRITE setXStart NOTIFY xStartChanged)
  Q_PROPERTY(double yStart READ yStart WRITE setYStart NOTIFY yStartChanged)
  Q_PROPERTY(double xEnd READ xEnd WRITE setXEnd NOTIFY xEndChanged)
  Q_PROPERTY(double yEnd READ yEnd WRITE setYEnd NOTIFY yEndChanged)
  Q_PROPERTY(double resolution READ resolution WRITE setResolution NOTIFY
                 resolutionChanged)
  Q_PROPERTY(double attractorPosThreshold READ attractorPosThreshold WRITE
                 setAttractorPosThreshold NOTIFY attractorPosThresholdChanged)
  Q_PROPERTY(double midPosThreshold READ midPosThreshold WRITE
                 setMidPosThreshold NOTIFY midPosThresholdChanged)
  Q_PROPERTY(double convergeTimeThreshold READ convergeTimeThreshold WRITE
                 setConvergeTimeThreshold NOTIFY convergeTimeThresholdChanged)
  Q_PROPERTY(QColor midConvergeColor READ midConvergeColor WRITE
                 setMidConvergeColor NOTIFY midConvergeColorChanged)
  Q_PROPERTY(QColor outOfBoundsColor READ outOfBoundsColor WRITE
                 setOutOfBoundsColor NOTIFY outOfBoundsColorChanged)

public:
  explicit PendulumMapModel(QObject *parent = 0);

  double xStart() const;
  double yStart() const;
  double xEnd() const;
  double yEnd() const;
  double resolution() const;
  double attractorPosThreshold() const;
  double midPosThreshold() const;
  double convergeTimeThreshold() const;
  QColor midConvergeColor() const;
  QColor outOfBoundsColor() const;

  void setXStart(double xStart);
  void setYStart(double yStart);
  void setXEnd(double xEnd);
  void setYEnd(double yEnd);
  void setResolution(double resolution);
  void setAttractorPosThreshold(double attractorPosThreshold);
  void setMidPosThreshold(double midPosThreshold);
  void setConvergeTimeThreshold(double convergeTimeThreshold);
  void setMidConvergeColor(QColor midConvergeColor);
  void setOutOfBoundsColor(QColor outOfBoundsColor);

signals:
  void xStartChanged(double xStart);
  void yStartChanged(double yStart);
  void xEndChanged(double xEnd);
  void yEndChanged(double yEnd);
  void resolutionChanged(double resolution);
  void attractorPosThresholdChanged(double attractorPosThreshold);
  void midPosThresholdChanged(double midPosThreshold);
  void convergeTimeThresholdChanged(double convergeTimeThreshold);
  void midConvergeColorChanged(QColor midConvergeColor);
  void outOfBoundsColorChanged(QColor outOfBoundsColor);

private:
  double m_xStart;
  double m_yStart;
  double m_xEnd;
  double m_yEnd;
  double m_resolution;
  double m_attractorPosThreshold;
  double m_midPosThreshold;
  double m_convergeTimeThreshold;
  QColor m_midConvergeColor;
  QColor m_outOfBoundsColor;
};
} // namespace staticpendulum
#endif // PENDULUMMAPMODEL_H
