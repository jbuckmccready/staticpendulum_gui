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

RowLayout {
  id: root
  spacing: -1 // creates grid effect with rectangle border overlap
  z: 2 // keeps the header above the listview delegate items

  height: 25
  property font headerFont: Qt.font({
              weight: Font.Black,
              pointSize: 14,
              bold: true
            })

  Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    border.color: "black"
    border.width: 1
    Text {
      anchors.centerIn: parent
      horizontalAlignment: Qt.AlignHCenter
      text: "XPosition"
      font: root.headerFont
    }
  }
  Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    border.color: "black"
    border.width: 1
    Text {
      anchors.centerIn: parent
      horizontalAlignment: Qt.AlignHCenter
      text: "YPosition"
      font: root.headerFont
    }
  }
  Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    border.color: "black"
    border.width: 1
    Text {
      anchors.centerIn: parent
      horizontalAlignment: Qt.AlignHCenter
      text: "ForceCoef"
      font: root.headerFont
    }
  }
  Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    border.color: "black"
    border.width: 1
    Text {
      anchors.centerIn: parent
      horizontalAlignment: Qt.AlignHCenter
      text: "Color"
      font: root.headerFont
    }
  }
}
