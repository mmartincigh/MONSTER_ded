import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    property bool enabled: processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed

    width: childrenRect.width
    height: childrenRect.height

    Connections {
        target: destinationPathManager
        onEditTextChanged: { destinationPathComboBox.editText = editText }
    }

    Text {
        id: destinationPathLabel

        anchors.left: parent.left
        anchors.verticalCenter: destinationPathComboBox.verticalCenter

        text: qsTr("destination path")
    }

    ComboBox {
        id: destinationPathComboBox

        height: 25
        anchors.top: parent.top
        anchors.left: destinationPathLabel.right
        anchors.leftMargin: 20
        anchors.right: destinationPathBrowseButton.left
        anchors.rightMargin: 20

        model: destinationPathManager.pathModel
        editable: true
        enabled: main.enabled

        onEditTextChanged: { destinationPathManager.onUpdateEditText(editText) }

        Text {
            id: destinationPathOverlayText

            anchors.centerIn: parent

            text: qsTr("enter or select destination path")
            color: "grey"
            opacity: parent.activeFocus || parent.editText.length > 0 ? 0 : 1

            Behavior on opacity {
                NumberAnimation { duration: 125 }
            }
        }
    }

    Button {
        id: destinationPathBrowseButton

        anchors.right: parent.right
        anchors.verticalCenter: destinationPathComboBox.verticalCenter

        text: qsTr("browse")
        enabled: main.enabled

        onClicked: { fileDialog.open() }
    }

    FileDialog {
        id: fileDialog

        title: qsTr("Select a folder")
        selectFolder: true
        folder: destinationPathManager.isPathUrlValid ? destinationPathManager.pathUrl : shortcuts.documents

        onAccepted: { destinationPathManager.onUpdateEditText(folder) }
    }
}
