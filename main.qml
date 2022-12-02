import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Peach

ApplicationWindow {
    id: root
    height: 540
    title: viewModel.filepath.length > 0 ? ("Peach - " + viewModel.filepath) : "Peach"
    visible: true
    width: 960
    property real lineWidth: 1
    property real graphWidth: lineWidth * viewModel.pktInfosSize

    Component.onCompleted: {
        histogram.setViewModel(viewModel)
    }

    MainViewModel {
        id: viewModel
        onLoadedChanged: {
            seekBar.value = 0
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            Action {
                text: "&Open"
                shortcut: "ctrl+o"
                onTriggered: {
                    fileDialog.open()
                }
            }
            Action {
                text: "Close"
                enabled: viewModel.loaded
                onTriggered: {
                    viewModel.unload()
                }
            }
            MenuSeparator {}
            Action {
                text: "&Quit"
                shortcut: "ctrl+q"
                onTriggered: {
                    root.close()
                }
            }
        }
    }

    Rectangle {
        id: histogramPanel
        anchors.fill: parent
        color: "#1A1A1A"

        PacketHistogram {
            id: histogram
            lineWidth: root.lineWidth
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: seekBar.top
            }
        }
        Slider {
            id: seekBar
            visible: viewModel.loaded
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            from: 0
            to: viewModel.pktInfosSize - width / root.lineWidth
            stepSize: 1
            height: 32
            leftPadding: 0
            rightPadding: 0
            background: Rectangle {
                anchors.fill: parent
                color: "#444444"
                border.color: "cyan"
            }
            handle: Rectangle {
                color: "cyan"
                height: parent.height
                width: graphWidth > 0 ? parent.width * parent.width / graphWidth : 10
                x: seekBar.leftPadding + seekBar.visualPosition * (seekBar.availableWidth - width)
            }
            onValueChanged: {
                histogram.seek(value)
            }
        }
    }

    FileDialog {
        id: fileDialog
        onAccepted: {
            viewModel.load(selectedFile)
        }
    }

    WheelHandler {
        enabled: viewModel.loaded
        onWheel: function (wheel) {
            if (wheel.modifiers & Qt.ControlModifier) {
                let w = root.lineWidth
                if (wheel.angleDelta.y > 0) {
                    w *= 2
                } else if (wheel.angleDelta.y < 0) {
                    w /= 2
                }
                if (w < 0.1) {
                    w = 0.1
                }
                if (w > 4) {
                    w = 4
                }
                root.lineWidth = w
            } else {
                if (wheel.angleDelta.y > 0) {
                    seekBar.value += width / root.lineWidth / 3
                } else if (wheel.angleDelta.y < 0) {
                    seekBar.value -= width / root.lineWidth / 3
                }
            }
        }
    }
}
