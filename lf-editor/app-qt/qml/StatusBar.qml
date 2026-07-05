import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: "#181818"
    implicitHeight: 24

    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        text: transport.statusMessage
        color: "#888888"
        font.pixelSize: 12
    }
}
