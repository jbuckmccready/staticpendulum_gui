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
import QtQuick 2.7
import QtQuick.Layouts 1.3

GridLayout {
  columns: 2
  rows: 8
  rowSpacing: 3

  property bool isValid: xStartField.acceptableInput && yStartField.acceptableInput &&
                         xEndField.acceptableInput && yEndField.acceptableInput &&
                         resolutionField.acceptableInput && attractorPosThreshholdField.acceptableInput &&
                         midPosThreshholdField.acceptableInput && convergeTimeThresholdField.acceptableInput

  LabelWithHoverToolTip {
    Layout.row: 0
    Layout.column: 0
    text: "X Start:"
    toolTipText: "Starting value on the x-axis."
  }

  TextFieldWithNumericValidation {
    id: xStartField
    Layout.row: 0
    Layout.column: 1
    bindedModelValue: pendulumMapModel.xStart
    onTextAsDoubleChanged: pendulumMapModel.xStart = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 1
    Layout.column: 0
    text: "Y Start:"
    toolTipText: "Starting value on the y-axis."
  }

  TextFieldWithNumericValidation {
    id: yStartField
    Layout.row: 1
    Layout.column: 1
    bindedModelValue: pendulumMapModel.yStart
    onTextAsDoubleChanged: pendulumMapModel.yStart = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 2
    Layout.column: 0
    text: "X End:"
    toolTipText: "End value on the x-axis."
  }

  TextFieldWithNumericValidation {
    id: xEndField
    Layout.row: 2
    Layout.column: 1
    bindedModelValue: pendulumMapModel.xEnd
    onTextAsDoubleChanged: pendulumMapModel.xEnd = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 3
    Layout.column: 0
    text: "Y End:"
    toolTipText: "End value on the y-axis."
  }

  TextFieldWithNumericValidation {
    id: yEndField
    Layout.row: 3
    Layout.column: 1
    bindedModelValue: pendulumMapModel.yEnd
    onTextAsDoubleChanged: pendulumMapModel.yEnd = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 4
    Layout.column: 0
    text: "Resolution:"
    toolTipText: "Spacing between x and y points to be integrated."
  }

  TextFieldWithNumericValidation {
    id: resolutionField
    Layout.row: 4
    Layout.column: 1
    bindedModelValue: pendulumMapModel.resolution
    onTextAsDoubleChanged: pendulumMapModel.resolution = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 5
    Layout.column: 0
    text: "Attractor Pos Threshold:"
    toolTipText: "How close along both the X and Y axis the pendulum head must be to be counted as on an attractor."
  }

  TextFieldWithNumericValidation {
    id: attractorPosThreshholdField
    Layout.row: 5
    Layout.column: 1
    bindedModelValue: pendulumMapModel.attractorPosThreshold
    onTextAsDoubleChanged: pendulumMapModel.attractorPosThreshold = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 6
    Layout.column: 0
    text: "Mid Pos Threshhold:"
    toolTipText: "How close along both the X and Y axis the pendulum head must be to be counted as at (0,0)."
  }

  TextFieldWithNumericValidation {
    id: midPosThreshholdField
    Layout.row: 6
    Layout.column: 1
    bindedModelValue: pendulumMapModel.midPosThreshold
    onTextAsDoubleChanged: pendulumMapModel.midPosThreshold = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 7
    Layout.column: 0
    text: "Converge Time Threshold:"
    toolTipText: "How long the pendulum head must remain within a converge threshold to be considered converged and stop integration."
  }

  TextFieldWithNumericValidation {
    id: convergeTimeThresholdField
    Layout.row: 7
    Layout.column: 1
    bindedModelValue: pendulumMapModel.convergeTimeThreshold
    onTextAsDoubleChanged: pendulumMapModel.convergeTimeThreshold = textAsDouble
  }
}
