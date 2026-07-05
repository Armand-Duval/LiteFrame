import QtQuick
import QtQuick.Controls
import LiteFrame 1.0

Item {
    id: root

    signal openFileRequested()

    PreviewViewport {
        id: previewViewport
        anchors.fill: parent
        hasMedia: transport.hasMedia

        Component.onCompleted: previewViewport.bindPreviewController(preview)

        DropArea {
            anchors.fill: parent
            onEntered: (drag) => { drag.acceptProposedAction() }

            onDropped: (drop) => {
                if (drop.hasUrls && drop.urls.length > 0) {
                    transport.openMedia(drop.urls[0])
                }
            }

            Rectangle {
                anchors.centerIn: parent
                width: placeholderColumn.width + 48
                height: placeholderColumn.height + 32
                radius: 8
                color: "#80000000"
                visible: !transport.hasMedia

                Column {
                    id: placeholderColumn
                    anchors.centerIn: parent
                    spacing: 12

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Drop media here")
                        color: "#dddddd"
                        font.pixelSize: 18
                    }
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Open File...")
                        onClicked: root.openFileRequested()
                    }
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Import...")
                        onClicked: root.openFileRequested()
                    }
                }
            }
        }
    }
}
