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
        onAccepted: transport.openMedia(selectedFile.toString())
    }

    FileDialog {
        id: exportDialog
        title: qsTr("Export Timeline")
        fileMode: FileDialog.SaveFile
        nameFilters: [qsTr("MP4 Video (*.mp4)"), qsTr("All Files (*)")]
        onAccepted: transport.exportTimeline(selectedFile.toString())
    }

    Connections {
        target: transport
        function onOpenFailed(message) { errorDialog.text = message; errorDialog.open() }
        function onExportFailed(message) { errorDialog.text = message; errorDialog.open() }
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

        PreviewPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            onOpenFileRequested: openDialog.open()
        }

        TransportBar {
            Layout.fillWidth: true
        }

        StatusBar {
            Layout.fillWidth: true
        }
    }
}
