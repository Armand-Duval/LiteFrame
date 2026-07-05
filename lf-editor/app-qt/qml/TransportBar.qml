import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#1e1e1e"
    implicitHeight: 56

    function formatTime(seconds) {
        if (!(seconds >= 0) || isNaN(seconds)) {
            return "00:00"
        }
        const total = Math.floor(seconds)
        const mins = Math.floor(total / 60)
        const secs = total % 60
        return String(mins).padStart(2, "0") + ":" + String(secs).padStart(2, "0")
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Button {
            text: qsTr("Play")
            enabled: transport.hasMedia
            onClicked: transport.play()
        }
        Button {
            text: qsTr("Pause")
            enabled: transport.hasMedia
            onClicked: transport.pause()
        }

        Slider {
            id: seekSlider
            Layout.fillWidth: true
            from: 0
            to: transport.duration > 0 ? transport.duration : 1
            value: transport.position
            enabled: transport.hasMedia && transport.duration > 0

            onPressedChanged: transport.seeking = pressed
            onMoved: transport.seek(value)

            Connections {
                target: transport
                function onPositionChanged() {
                    if (!seekSlider.pressed) {
                        seekSlider.value = transport.position
                    }
                }
                function onDurationChanged() {
                    seekSlider.to = transport.duration > 0 ? transport.duration : 1
                }
            }
        }

        Label {
            text: root.formatTime(transport.position) + " / " + root.formatTime(transport.duration)
            color: "#cccccc"
            font.family: "Menlo, Monaco, monospace"
        }
    }
}
