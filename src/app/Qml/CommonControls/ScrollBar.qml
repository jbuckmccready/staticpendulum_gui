import QtQuick 2.7
import QtQuick.Controls 2.0
ScrollBar {
  id: scrollBar
  active: true
  property alias backgroundHeight: sbBackground.implicitHeight
  contentItem: Rectangle {
    implicitWidth: 10
    implicitHeight: 100
    radius: width / 2
    color: "darkgreen"
    border.width: scrollBar.pressed ? 2 : 0
    border.color: "lightgreen"
  }
  background: Rectangle {
    id: sbBackground
    implicitWidth: 10
    color: "grey"
  }
}
