import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Peach

ApplicationWindow {
    id: root
    height: 540
    title: viewModel.filepath.length > 0 ? ("Peach - " + viewModel.filepath) : "Peach"
    visible: true
    width: 960
    property real lineWidth: 1

    Component.onCompleted: {
        histogram.setViewModel(viewModel)
    }

    MainViewModel {
        id: viewModel
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
        anchors.fill: parent
        color: "#1A1A1A"

        PacketHistogram {
            id: histogram
            lineWidth: root.lineWidth
            anchors.fill: parent
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
            }
        }
    }
}
