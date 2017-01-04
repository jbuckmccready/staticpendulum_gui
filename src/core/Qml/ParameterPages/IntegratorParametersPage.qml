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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import CommonControls 1.0
import ModelsRepo 1.0

GridLayout {
  columns: 2
  rows: 5
  rowSpacing: 3

  property bool isValid: startingStepSizeField.acceptableInput && maximumStepSizeField.acceptableInput &&
                         relativeTolField.acceptableInput && absoluteTolField.acceptableInput &&
                         threadCountField.acceptableInput;

  LabelWithHoverToolTip {
    Layout.row: 0
    Layout.column: 0
    text: "Starting Step Size:"
    toolTipText: "Step size for first integration step."
  }

  TextFieldWithNumericValidation {
    id: startingStepSizeField
    Layout.row: 0
    Layout.column: 1
    bindedModelValue: ModelsRepo.integratorModel.startingStepSize
    onTextAsDoubleChanged: ModelsRepo.integratorModel.startingStepSize = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 1
    Layout.column: 0
    text: "Maximum Step Size:"
    toolTipText: "Maximum integration step size allowed."
  }

  TextFieldWithNumericValidation {
    id: maximumStepSizeField
    Layout.row: 1
    Layout.column: 1
    bindedModelValue: ModelsRepo.integratorModel.maximumStepSize
    onTextAsDoubleChanged: ModelsRepo.integratorModel.maximumStepSize = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 2
    Layout.column: 0
    text: "Relative Tolerance:"
    toolTipText: "Parameter to control relative error allowed."
  }

  TextFieldWithNumericValidation {
    id: relativeTolField
    Layout.row: 2
    Layout.column: 1
    bindedModelValue: ModelsRepo.integratorModel.relativeTolerance
    onTextAsDoubleChanged: ModelsRepo.integratorModel.relativeTolerance = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 3
    Layout.column: 0
    text: "Absolute Tolerance:"
    toolTipText: "Paramter to control absolute error allowed."
  }

  TextFieldWithNumericValidation {
    id: absoluteTolField
    Layout.row: 3
    Layout.column: 1
    bindedModelValue: ModelsRepo.integratorModel.absoluteTolerance
    onTextAsDoubleChanged: ModelsRepo.integratorModel.absoluteTolerance = textAsDouble
  }

  LabelWithHoverToolTip {
    Layout.row: 4
    Layout.column: 0
    text: "Thread Count:"
    toolTipText: "Number of threads to run in parallel when integrating all the points."
  }

  TextFieldWithNumericValidation {
    id: threadCountField
    Layout.row: 4
    Layout.column: 1
    bindedModelValue: ModelsRepo.integratorModel.threadCount
    onTextAsDoubleChanged: ModelsRepo.integratorModel.threadCount = textAsDouble
  }
}

