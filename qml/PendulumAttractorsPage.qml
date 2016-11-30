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
  id: rootColumn
  anchors.fill: parent
  property bool isValid: attractorsListView.currentItem.hasValidInput

  ListView {
    id: attractorsListView
    Layout.fillHeight: true
    Layout.fillWidth: true
    contentWidth: rootColumn.width - scrollBar.width // share space with scroll bar

    spacing: -1 // creates the grid effect with rectangle borders in delegate

    ScrollBar.vertical: ScrollBar {
      id: scrollBar
      active: true
      contentItem: Rectangle {
        implicitWidth: 10
        implicitHeight: 100
        radius: width / 2
        color: "darkgreen"
        border.width: scrollBar.pressed ? 2 : 0
        border.color: "lightgreen"
      }
      background: Rectangle {
        implicitHeight: attractorsListView.height
        implicitWidth: 10
        color: "grey"
      }
    }
    clip: true // keeps the listview contents within its bounding box
    boundsBehavior: Flickable.StopAtBounds // disables overshoot and rebound

    model: pendulumSystemModel.attractorList

    headerPositioning: ListView.OverlayHeader
    header: PendulumAttractorsHeader {
      width: attractorsListView.contentWidth
    }

    footerPositioning: ListView.OverlayFooter
    footer: Rectangle {
      implicitHeight: 1
      implicitWidth: attractorsListView.contentWidth
      color: "black"
    }

    delegate: Loader {
      // place this delegate item above others if selected (for clean border overlap when selected)
      z: attractorsListView.currentIndex === model.index ? 1 : -1

      // property to check if delegate has valid input
      property bool hasValidInput: item.hasValidInput

      sourceComponent:  Component {
        id: attractorDelegate
        Rectangle {
          id: rootRect
          height: xPositionField.height
          width: attractorsListView.contentWidth
          color: attractorsListView.currentIndex === model.index ? "lightgreen" : "white"
          property bool hasValidInput: (xPositionField.acceptableInput && yPositionField.acceptableInput && forceCoefficientField.acceptableInput)

          // mouse area allows for clicking any regions in the row that
          // are not covered by a control in the focus scope
          MouseArea {
            anchors.fill: parent
            onClicked: focusScope.forceActiveFocus()
          }

          // Focus scope for the delegate
          FocusScope {
            id: focusScope
            anchors.fill: parent
            onActiveFocusChanged: {
              if (!activeFocus)
                return;

              if (attractorsListView.currentItem.hasValidInput) {
                // just change the current index to match focus
                attractorsListView.currentIndex = model.index;
                return;
              }

              // currentItem does not have valid input, prevent changing the current index and center
              // on the invalid delegate
              attractorsListView.currentItem.forceActiveFocus();
              attractorsListView.positionViewAtIndex(attractorsListView.currentIndex, ListView.Center);
            }

            RowLayout {
              anchors.fill: parent
              spacing: -1 // creates the grid effect with rectangle borders
              // enable only if current item or input is valid, adds nice visual cue when input is invalid
              enabled: attractorsListView.currentIndex === model.index || attractorsListView.currentItem.hasValidInput

              TextFieldWithNumericValidation {
                id: xPositionField
                Layout.fillWidth: true
                focus: true
                bindedModelValue: model.xPosition
                onTextAsDoubleChanged: model.xPosition = textAsDouble
                z: (activeFocus || !xPositionField.acceptableInput) ? 1 : -1
              }
              TextFieldWithNumericValidation {
                id: yPositionField
                Layout.fillWidth: true
                bindedModelValue: model.yPosition
                onTextAsDoubleChanged: model.yPosition = textAsDouble

                z: (activeFocus || !yPositionField.acceptableInput) ? 1 : -1
              }
              TextFieldWithNumericValidation {
                id: forceCoefficientField
                Layout.fillWidth: true
                bindedModelValue: model.forceCoefficient
                onTextAsDoubleChanged: model.forceCoefficient = textAsDouble
                z: (activeFocus || !forceCoefficientField.acceptableInput) ? 1 : -1
              }
              Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                opacity: enabled ? 1 : 0.2
                color: enabled ? "transparent" : "#353637"
                border.color: enabled ? (colorSelectorButton.isPickingColor ? "#0066ff" : "lightgrey") : "transparent"
                border.width: colorSelectorButton.isPickingColor ? 2 : 1

                Component.onCompleted: {
                  implicitWidth = xPositionField.implicitWidth;
                  implicitHeight = xPositionField.implicitHeight;
                }

                Button {
                  id: colorSelectorButton
                  property bool isPickingColor: false
                  anchors.fill: parent
                  anchors.margins: 10
                  z: activeFocus ? 1 : -1
                  onClicked: {
                    colorSelectorButton.isPickingColor = true;

                    // Load the color dialog if it's not loaded yet
                    colorDialogLoader.active = true;
                    // Handler if color dialog is accepted
                    function onAccepted() {
                        model.color = colorDialogLoader.item.color;
                        colorSelectorButton.isPickingColor = false;
                        disconnectHandlers();
                    }

                    // Handler if color dialog is rejected
                    function onRejected() {
                        colorSelectorButton.isPickingColor = false;
                        disconnectHandlers();
                    }

                    function disconnectHandlers() {
                        colorDialogLoader.item.accepted.disconnect(onAccepted);
                        colorDialogLoader.item.rejected.disconnect(onRejected);
                    }
                    // Connect handlers and open dialog
                    colorDialogLoader.item.accepted.connect(onAccepted);
                    colorDialogLoader.item.rejected.connect(onRejected);
                    colorDialogLoader.item.color = model.color;
                    colorDialogLoader.item.open();
                  }
                  background: Rectangle {
                    anchors.fill: parent
                    color: model.color
                    border.color: "grey"
                    border.width: 3
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  RowLayout {
    id: buttonRow
    Layout.fillHeight: true
    Layout.fillWidth: true
    spacing: 5
    Button {
      text: "Add Attractor"
      onClicked: pendulumSystemModel.attractorList.addAttractor(0, 0, 0, 1, "red")
    }
    Button {
      text: "Delete Attractor"
      onClicked: {
        // check if item is visible and if it is not then position the view so it is visible
        // and do not delete immediately
        var topItemIndex = attractorsListView.indexAt(attractorsListView.contentX,
                                                      attractorsListView.contentY);
        if (topItemIndex > attractorsListView.currentIndex) {
          attractorsListView.positionViewAtIndex(attractorsListView.currentIndex, ListView.Center);
          return;
        }

        var bottomItemIndex = attractorsListView.indexAt(attractorsListView.contentX,
                                                         attractorsListView.contentY + attractorsListView.height - 20);
        if (bottomItemIndex < attractorsListView.currentIndex && bottomItemIndex !== -1) {
          attractorsListView.positionViewAtIndex(attractorsListView.currentIndex, ListView.Center);
          return;
        }

        pendulumSystemModel.attractorList.removeAttractor(attractorsListView.currentIndex);
      }
    }

  }
  Loader {
    id: colorDialogLoader
    // active == false for lazy loading, set to true when used
    active: false

    source: "ColorDialog.qml"
  }
}


