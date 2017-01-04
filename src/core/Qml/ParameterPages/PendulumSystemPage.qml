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

ColumnLayout {
  property bool isValid: attractorsPage.isValid && parametersPage.isValid

  TabBar {
    id: bar
    Layout.fillWidth: true
    TabButton {
      id: attractorsPageButton
      text: qsTr("Attractors")
      background: Rectangle {
        implicitHeight: 40
        color: attractorsPageButton.down ? (attractorsPageButton.checked ? "#e4e4e4" : "#585a5c") : (attractorsPageButton.checked ? "transparent" : "#353637")
        border.width: 2
        border.color: attractorsPage.isValid ? "transparent" : "red"
      }
    }
    TabButton {
      id: parametersPageButton
      text: qsTr("Pedulum Parameters")
      background: Rectangle {
        implicitHeight: 40
        color: parametersPageButton.down ? (parametersPageButton.checked ? "#e4e4e4" : "#585a5c") : (parametersPageButton.checked ? "transparent" : "#353637")
        border.width: 2
        border.color: parametersPage.isValid ? "transparent" : "red"
      }
    }
  }

  StackLayout {
    Layout.fillHeight: true
    Layout.fillWidth: true
    currentIndex: bar.currentIndex
    PendulumAttractorsPage {
      id: attractorsPage
    }
    PendulumParametersPage {
      id: parametersPage
    }
  }
}
