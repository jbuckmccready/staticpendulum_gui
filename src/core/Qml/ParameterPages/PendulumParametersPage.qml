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
import CommonControls 1.0
import ModelsRepo 1.0
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

GridLayout {
  columns: 2
  rows: 5
  rowSpacing: 3

  property bool isValid: distanceField.acceptableInput && massField.acceptableInput &&
                         gravityField.acceptableInput && dragField.acceptableInput &&
                         lengthField.acceptableInput;

  LabelWithHoverToolTip {
    Layout.row: 0
    Layout.column: 0
    text: "Pendulum Distance:"
    toolTipText: "Distance from the pendulum head to the base plate."
  }

  TextFieldWithNumericValidation {
    id: distanceField
    Layout.row: 0
    Layout.column: 1
    bindedModelValue: ModelsRepo.pendulumSystemModel.distance
    onTextAsDoubleChanged: ModelsRepo.pendulumSystemModel.distance = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 1
    Layout.column: 0
    text: "Pendulum Mass:"
    toolTipText: "Mass of the pendulum bob/head."
  }

  TextFieldWithNumericValidation {
    id: massField
    Layout.row: 1
    Layout.column: 1
    bindedModelValue: ModelsRepo.pendulumSystemModel.mass
    onTextAsDoubleChanged: ModelsRepo.pendulumSystemModel.mass = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 2
    Layout.column: 0
    text: "Gravity:"
    toolTipText: "Gravity force influencing the pendulum."
  }

  TextFieldWithNumericValidation {
    id: gravityField
    Layout.row: 2
    Layout.column: 1
    bindedModelValue: ModelsRepo.pendulumSystemModel.gravity
    onTextAsDoubleChanged: ModelsRepo.pendulumSystemModel.gravity = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 3
    Layout.column: 0
    text: "Pendulum Drag:"
    toolTipText: "Linear drag force influencing the pendulum."
  }

  TextFieldWithNumericValidation {
    id: dragField
    Layout.row: 3
    Layout.column: 1
    bindedModelValue: ModelsRepo.pendulumSystemModel.drag
    onTextAsDoubleChanged: ModelsRepo.pendulumSystemModel.drag = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 4
    Layout.column: 0
    text: "Pendulum Length:"
    toolTipText: "Length of the pendulum."
  }

  TextFieldWithNumericValidation {
    id: lengthField
    Layout.row: 4
    Layout.column: 1
    bindedModelValue: ModelsRepo.pendulumSystemModel.length
    onTextAsDoubleChanged: ModelsRepo.pendulumSystemModel.length = textAsDouble
  }
}
