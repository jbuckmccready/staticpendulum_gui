import ModelsRepo 1.0
import CommonControls 1.0 as CC
import QtQuick 2.7
import Qt.labs.folderlistmodel 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
Popup {
  id: root
  modal: true
  closePolicy: Popup.CloseOnPressOutside
  background: Rectangle {
    color: "white"
    border.color: "#444"
    anchors.fill: parent
    implicitHeight: 300

    // Mouse area to handle/block mouse hover events
    MouseArea {
      anchors.fill: parent
      hoverEnabled: true
    }
  }

  ColumnLayout {
    id: rootColumn
    anchors.fill: parent

    // Listview of files
    ListView {
      id: parameterSetsListView
      Layout.fillHeight: true
      Layout.fillWidth: true
      contentWidth: rootColumn.width - scrollBar.width // share space with scroll bar

      ScrollBar.vertical: CC.ScrollBar {
        id: scrollBar
        backgroundHeight: parameterSetsListView.height
      }

      model: FolderListModel {
        id: folderListModel
        showDirs: false

        // only looking for json files
        nameFilters: [ "*.json" ]

        // Set to the jsonFilesDirPath directory
        folder: "file:///" + ModelsRepo.jsonFilesDirPath
      }

      delegate: Rectangle {
        id: jsonFileDelegate
        property string fileName: model.fileName
        width: parameterSetsListView.contentWidth
        color: parameterSetsListView.currentIndex === model.index ? "lightgreen" : "white"
        implicitHeight: fileNameText.implicitHeight
        implicitWidth: fileNameText.implicitWidth
        Text {
          id: fileNameText
          font.pointSize: 14
          leftPadding: 2
          // file name without the .json
          text: jsonFileDelegate.fileName.substr(0, model.fileName.length - 5)
        }

        MouseArea {
          anchors.fill: parent
          onClicked: parameterSetsListView.currentIndex = index
        }
      }

      function getSelectedAbsolutePath() {
        if (!currentItem)
          return "";

        return ModelsRepo.jsonFilesDirPath + currentItem.fileName;
      }
    }

    // Buttons for actions
    RowLayout {
      id: buttonRow
      spacing: 5
      Button {
        text: "Save Current"
        onClicked: saveFilePopup.open()
      }
      Button {
        enabled: parameterSetsListView.currentIndex !== -1 ? true : false
        text: "Load"
        onClicked: ModelsRepo.loadJsonFile(parameterSetsListView.getSelectedAbsolutePath())
      }
      Button {
        enabled: parameterSetsListView.currentIndex !== -1 ? true : false
        text: "Delete"
        onClicked: deleteConfirmPopup.open()
      }
      Button {
        text: "Back"
        onClicked: root.close()
      }
    }

    // Save file popup
    Popup {
      id: saveFilePopup
      modal: true
      closePolicy: Popup.NoAutoClose
      onOpened: saveFileTitleText.text = "Enter a name for the saved parameter set:"
      ColumnLayout {
        Text {
          id: saveFileTitleText
          font.pointSize: 12
        }
        TextField {
          id: fileNameInputField
          Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter
          spacing: 5
          Button {
            text: "OK"
            onClicked: {
              if (fileNameInputField.text === "") {
                return;
              }
              // absolute file path
              var absFilePath = ModelsRepo.jsonFilesDirPath + fileNameInputField.text + ".json";

              if (folderListModel.indexOf("file:///" + absFilePath) !== -1) {
                // file already exists
                saveFileTitleText.text = "<u>File already exists, please enter another name:</u>";
                return;
              }

              if (!ModelsRepo.saveJsonFile(absFilePath)) {
                // faild to write to disk
                saveFileTitleText.text = "<font color=\"red\"><u>Failed to write to disk, permission error or disk failure.</u></font>";
                return;
              }

              // successfully saved
              saveFilePopup.close();
            }
          }
          Button {
            text: "Cancel"
            onClicked: saveFilePopup.close()
          }
        }
      }

    }

    // Delete confirmation popup
    Popup {
      id: deleteConfirmPopup
      modal: true
      closePolicy: Popup.NoAutoClose
      onOpened: confirmMsgText.text = "Are you sure you wish to delete \""  + parameterSetsListView.currentItem.fileName + "\"?"
      ColumnLayout {
        Text {
          id: confirmMsgText
          font.pointSize: 12
        }
        RowLayout {
          Layout.alignment: Qt.AlignHCenter
          spacing: 5
          Button {
            text: "Yes"
            onClicked: {
              ModelsRepo.deleteJsonFile(parameterSetsListView.getSelectedAbsolutePath());
              deleteConfirmPopup.close();
            }
          }
          Button {
            text: "No"
            onClicked: deleteConfirmPopup.close()
          }
        }
      }
    }
  }
}
