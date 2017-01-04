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
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QmlHelpers 1.0
import ParameterPages 1.0
import ModelsRepo 1.0

ApplicationWindow {
  id: applicationWindow
  title: qsTr("staticpendulum")
  width: 640
  height: 480
  minimumHeight: 480
  minimumWidth: 640
  visible: true

  Connections {
    // connect to the aboutToQuit signal to cancel integration to prevent
    // concurrent threads causing crash on exit if busy integrating
    target: Qt.application
    onAboutToQuit: integrator.cancelIntegration()
  }

  header: ToolBar {
    id: applicationToolBar
    RowLayout {
      ToolButton {
        text: qsTr("Load")
        onClicked: {
          ModelsRepo.loadJsonFile("test.json");
        }

      }
      ToolButton {
        text: qsTr("Save")
        onClicked: {
          ModelsRepo.saveJsonFile("test.json");
        }
      }
      ToolButton {
        text: qsTr("Integrate")
        onClicked: {
          if (!stackLayout.isValid) {
            invalidParamsPopup.open();
            return;
          }

          progressPopup.open();
          integrator.integrateMap(ModelsRepo.pendulumSystemModel, ModelsRepo.pendulumMapModel, ModelsRepo.integratorModel);
        }
      }
    }
  }

  SystemIntegrator {
    id: integrator
    onFinishedIntegration: {
      progressPopup.close();
      imagePopup.open();
    }
  }

  Popup {
    id: imagePopup
    x: parent.width/2 - contentWidth/2
    y: parent.height/2 - contentHeight/2
    modal: true
    dim: true
    Image {
      id: mapImage
      // turn cache off to enable loading new image by setting source = ""
      cache: false
      smooth: false
    }

    onOpened: mapImage.source = "file:///" + applicationDirPath + "/last_integrated.png"
    onClosed: mapImage.source = ""
  }

  Popup {
    id: progressPopup
    x: parent.width/2 - contentWidth/2
    y: parent.height/2 - contentHeight/2
    modal: true
    dim: true
    closePolicy: Popup.NoAutoClose

    ColumnLayout {
      ProgressBar {
        Layout.alignment: Qt.AlignHCenter
        from: integrator.progressMinimum
        to: integrator.progressMaximum
        value: integrator.progressValue
      }
      Text {
        Layout.alignment: Qt.AlignHCenter
        text: "Finished integrating %1 of %2 points using %3 threads.".arg(integrator.progressValue).arg(integrator.progressMaximum).arg(ModelsRepo.integratorModel.threadCount)
      }
      Button {
        Layout.alignment: Qt.AlignHCenter
        text: "Cancel"
        onClicked: integrator.cancelIntegration()
      }

    }
  }

  Popup {
    id: invalidParamsPopup
    x: parent.width/2 - contentWidth/2
    y: parent.height/2 - contentHeight/2
    modal: true
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    Text {
      text: "Invalid parameters for integration."
    }
  }

  footer: Rectangle {
    height: 5
    color: "lightgrey"
  }

  RowLayout {
    anchors.fill: parent

    ListView {
      id: navigationListView
      Layout.fillHeight: true
      model: stackLayout.children
      delegate: navigationDelegate
      property int titlePointSize: 14
      property int titleLeftPadding: 2

      Component.onCompleted: {
        // create temporary FontMetrics object to compute width required
        var fontMetrics = Qt.createQmlObject('import QtQuick 2.7; FontMetrics {}',
                     navigationListView);

        fontMetrics.font.pointSize = navigationListView.titlePointSize;
        var maxWidth = 0;
        for (var i = 0; i < stackLayout.children.length; ++i) {
          var titleWidth = fontMetrics.boundingRect(stackLayout.children[i].title).width;
          maxWidth = Math.max(maxWidth, titleWidth + navigationListView.titleLeftPadding + 5);
        }

        // set implicit width of the list view to not clip the largest width title
        navigationListView.implicitWidth = maxWidth;
        // destroy the temporary
        fontMetrics.destroy();
      }
    }

    Component {
      id: navigationDelegate
      Rectangle {
        id: delegateWrapper
        height: itemText.height
        width: ListView.view.width
        color: ListView.isCurrentItem ? "lightgreen" : "transparent"
        border.width: 2
        border.color: model.isValid ? "transparent" : "red"
        Text {
          id: itemText
          font.pointSize: navigationListView.titlePointSize
          text: model.title
          leftPadding: navigationListView.titleLeftPadding
        }
        MouseArea {
          anchors.fill: parent
          onClicked: navigationListView.currentIndex = model.index
        }
        //        Component.onCompleted: {
        //          if (ListView.view.implicitWidth < itemText.implicitWidth) {
        //            ListView.view.implicitWidth = itemText.implicitWidth;
        //          }
        //        }
      }
    }

    StackLayout {
      id: stackLayout
      Layout.fillHeight: true
      Layout.fillWidth: true
      currentIndex: navigationListView.currentIndex
      property bool isValid: pendulumSystemPage.isValid && integratorParametersPage.isValid && mapParametersPage.isValid

      PendulumSystemPage {
        id: pendulumSystemPage
        property string title: "System"
      }

      IntegratorParametersPage {
        id: integratorParametersPage
        property string title: "Integrator"
      }

      MapParametersPage {
        id: mapParametersPage
        property string title: "Map Parameters"
      }

    }
  }
}
