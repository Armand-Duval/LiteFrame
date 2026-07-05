import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import LiteFrame 1.0

ApplicationWindow {
    id: root
    width: 1280
    height: 720
    visible: true
    title: qsTr("LiteFrame Editor")
    color: "#121212"

    FileDialog {
        id: openDialog
        title: qsTr("Open Media")
        nameFilters: [
            qsTr("Media Files (*.mp4 *.mov *.mkv *.avi *.webm *.m4v)"),
            qsTr("All Files (*)")
        ]
        onAccepted: controller.openMedia(selectedFile.toString())
    }

    FileDialog {
        id: exportDialog
        title: qsTr("Export Timeline")
        fileMode: FileDialog.SaveFile
        nameFilters: [qsTr("MP4 Video (*.mp4)"), qsTr("All Files (*)")]
        onAccepted: controller.exportTimeline(selectedFile.toString())
    }

    Connections {
        target: controller
        function onOpenFailed(message) { errorDialog.text = message; errorDialog.open() }
        function onExportFailed(message) { errorDialog.text = message; errorDialog.open() }
        function onHasMediaChanged() { preview.hasMedia = controller.hasMedia }
    }

    MessageDialog {
        id: errorDialog
        title: qsTr("Error")
        buttons: MessageDialog.Ok
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                shortcut: StandardKey.Open
                onTriggered: openDialog.open()
            }
            Action {
                text: qsTr("&Import Media...")
                onTriggered: openDialog.open()
            }
            Action {
                text: qsTr("&Export...")
                shortcut: "Ctrl+E"
                onTriggered: exportDialog.open()
            }
            MenuSeparator {}
            Action {
                text: qsTr("E&xit")
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        PreviewViewport {
            id: preview
            Layout.fillWidth: true
            Layout.fillHeight: true
            hasMedia: controller.hasMedia

            Component.onCompleted: controller.attachPreview(preview)

            DropArea {
                anchors.fill: parent
                onEntered: (drag) => { drag.acceptProposedAction() }

                onDropped: (drop) => {
                    if (drop.hasUrls && drop.urls.length > 0) {
                        controller.openMedia(drop.urls[0])
                    }
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: placeholderColumn.width + 48
                    height: placeholderColumn.height + 32
                    radius: 8
                    color: "#80000000"
                    visible: !controller.hasMedia

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
                            onClicked: openDialog.open()
                        }
                        Button {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Import...")
                            onClicked: openDialog.open()
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            color: "#1e1e1e"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                Button {
                    text: qsTr("Play")
                    enabled: controller.hasMedia
                    onClicked: controller.play()
                }
                Button {
                    text: qsTr("Pause")
                    enabled: controller.hasMedia
                    onClicked: controller.pause()
                }

                Slider {
                    id: seekSlider
                    Layout.fillWidth: true
                    from: 0
                    to: controller.duration > 0 ? controller.duration : 1
                    value: controller.position
                    enabled: controller.hasMedia && controller.duration > 0

                    onPressedChanged: controller.seeking = pressed

                    onMoved: controller.seek(value)

                    Connections {
                        target: controller
                        function onPositionChanged() {
                            if (!seekSlider.pressed) {
                                seekSlider.value = controller.position
                            }
                        }
                        function onDurationChanged() {
                            seekSlider.to = controller.duration > 0 ? controller.duration : 1
                        }
                    }
                }

                Label {
                    text: formatTime(controller.position) + " / " + formatTime(controller.duration)
                    color: "#cccccc"
                    font.family: "Menlo, Monaco, monospace"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            color: "#181818"

            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 8
                text: controller.statusMessage
                color: "#888888"
                font.pixelSize: 12
            }
        }
    }

    function formatTime(seconds) {
        if (!(seconds >= 0) || isNaN(seconds)) {
            return "00:00"
        }
        const total = Math.floor(seconds)
        const mins = Math.floor(total / 60)
        const secs = total % 60
        return String(mins).padStart(2, "0") + ":" + String(secs).padStart(2, "0")
    }
}
