import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Peach

Window {
    height: 540
    title: viewModel.filepath.length > 0 ? ("Peach - " + viewModel.filepath) : "Peach"
    visible: true
    width: 960

    Component.onCompleted: {
        histogram.setViewModel(viewModel);
    }

    MainViewModel {
        id: viewModel
    }
    Row {
        id: controlBar
        height: childrenRect.height
        width: parent.width

        Button {
            text: "Open"

            onClicked: {
                fileDialog.open();
            }
        }
        Button {
            enabled: viewModel.loaded
            text: "Close"

            onClicked: {
                viewModel.close();
            }
        }
    }
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.top: controlBar.bottom
        color: "#1A1A1A"
        width: parent.width

        PacketHistogram {
            id: histogram
            anchors.fill: parent
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            viewModel.open(selectedFile);
        }
    }
}
