import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Peach

ApplicationWindow {
    id: root

    property real graphWidth: lineWidth * viewModel.pktInfosSize
    property real lineWidth: 1

    height: 540
    title: viewModel.filepath.length > 0 ? ("Peach - " + viewModel.filepath) : "Peach"
    visible: true
    width: 960

    menuBar: MenuBar {
        Menu {
            title: "File"

            Action {
                shortcut: "ctrl+o"
                text: "&Open"

                onTriggered: {
                    fileDialog.open();
                }
            }
            Action {
                enabled: viewModel.loaded
                text: "Close"

                onTriggered: {
                    viewModel.unload();
                }
            }
            MenuSeparator {
            }
            Action {
                shortcut: "ctrl+q"
                text: "&Quit"

                onTriggered: {
                    root.close();
                }
            }
        }
    }

    Component.onCompleted: {
        histogram.setViewModel(viewModel);
    }

    MainViewModel {
        id: viewModel
        onLoadedChanged: {
            seekBar.value = 0;
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
                bottom: seekBar.top
                left: parent.left
                right: parent.right
                top: parent.top
            }
            MouseArea {
                id: histogramMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
            Rectangle {
                color: "orange"
                height: parent.height
                opacity: 0.6
                visible: viewModel.loaded && histogramMouseArea.containsMouse
                width: Math.max(root.lineWidth, 1)
                x: Math.floor(histogramMouseArea.mouseX / root.lineWidth) * root.lineWidth
            }
        }
        Slider {
            id: seekBar
            from: 0
            height: 32
            leftPadding: 0
            rightPadding: 0
            stepSize: 1
            to: viewModel.pktInfosSize - width / root.lineWidth
            visible: viewModel.loaded

            background: Rectangle {
                anchors.fill: parent
                border.color: "cyan"
                color: "#444444"
            }
            handle: Rectangle {
                color: "cyan"
                height: parent.height
                width: graphWidth > 0 ? parent.width * parent.width / graphWidth : 10
                x: seekBar.leftPadding + seekBar.visualPosition * (seekBar.availableWidth - width)
            }

            onValueChanged: {
                histogram.seek(value);
            }

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            viewModel.load(selectedFile);
        }
    }
    WheelHandler {
        enabled: viewModel.loaded

        onWheel: function (wheel) {
            if (wheel.modifiers & Qt.ControlModifier) {
                let w = root.lineWidth;
                if (wheel.angleDelta.y > 0) {
                    w *= 2;
                } else if (wheel.angleDelta.y < 0 && histogram.width < root.graphWidth) {
                    w /= 2;
                }
                if (w < 0.125) {
                    w = 0.125;
                }
                if (w > 4) {
                    w = 4;
                }
                root.lineWidth = w;
            } else {
                if (wheel.angleDelta.y > 0) {
                    seekBar.value += width / root.lineWidth / 3;
                } else if (wheel.angleDelta.y < 0) {
                    seekBar.value -= width / root.lineWidth / 3;
                }
            }
        }
    }
}
