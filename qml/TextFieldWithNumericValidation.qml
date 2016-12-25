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

TextField {
  id: control
  validator: DoubleValidator { }
  placeholderText: "0.0"
  selectByMouse: true
  maximumLength: 12
  background: Rectangle {
    implicitWidth: 200
    implicitHeight: 40
    border.width: (control.activeFocus || !control.acceptableInput) ? 2 : 1
    color: control.enabled ? "transparent" : "#353637"
    border.color: !control.acceptableInput ? "red" : (control.activeFocus ? "#0066ff" : (control.enabled ? "#bdbebf" : "transparent"))
  }
  // Property to avoid writing back to binded model when reading from the model to update the text
  property bool isReadingFromModel: false;

  // Property to be binded to from the outside to the target model property
  property double bindedModelValue;

  // Property to be read from the outside to get the TextField text property as a valid double
  // if the text is not a valid then it will contain the last valid double.
  property double textAsDouble;

  onBindedModelValueChanged: {
    if (control.textAsDouble !== control.bindedModelValue) {
      control.isReadingFromModel = true;
      control.text = bindedModelValue;
      control.isReadingFromModel = false;
    }
  }

  onTextChanged: {
    if (control.isReadingFromModel)
      return;
    if (!control.acceptableInput)
      return;

    control.textAsDouble = parseFloat(control.text);
  }

  Component.onCompleted: {
    // if for any reason the text was not initialized (may be the case if the bindedModelValue starts as
    // 0.0 which will not trigger a property changed signal) then initialize the text from the model value
    if (!control.text) {
      control.isReadingFromModel = true;
      control.text = control.bindedModelValue;
      control.isReadingFromModel = false;
    }
  }
}
