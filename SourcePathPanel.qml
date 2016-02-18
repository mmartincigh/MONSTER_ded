import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    border.color: "red"

    property bool enabled: encryptionManager.state === Enums.ProcessState_Idle || encryptionManager.state === Enums.ProcessState_Stopped || encryptionManager.state === Enums.ProcessState_Completed

    width: childrenRect.width
    height: childrenRect.height

    Connections {
        target: sourcePathManager
        onEditTextChanged: { sourcePathComboBox.editText = editText }
    }

    Text {
        id: sourcePathLabel

        anchors.left: parent.left
        anchors.verticalCenter: sourcePathComboBox.verticalCenter

        text: qsTr("source path")
    }

    ComboBox {
        id: sourcePathComboBox

        height: 25
        anchors.top: parent.top
        anchors.left: sourcePathLabel.right
        anchors.leftMargin: 20
        anchors.right: sourcePathBrowseButton.left
        anchors.rightMargin: 20

        model: sourcePathManager.pathModel
        editable: true
        enabled: main.enabled

        onEditTextChanged: { sourcePathManager.onUpdateEditText(editText) }

        Text {
            id: sourcePathOverlayText

            anchors.centerIn: parent

            text: qsTr("enter or select the source path")
            color: "grey"
            opacity: parent.activeFocus || parent.editText.length > 0 ? 0 : 1

            Behavior on opacity {
                NumberAnimation { duration: 125 }
            }
        }
    }

    Button {
        id: sourcePathBrowseButton

        anchors.right: parent.right
        anchors.verticalCenter: sourcePathComboBox.verticalCenter

        text: qsTr("browse")
        enabled: main.enabled

        onClicked: { fileDialog.open() }
    }

    FileDialog {
        id: fileDialog

        title: qsTr("Select a folder")
        selectFolder: true
        folder: sourcePathManager.isPathUrlValid ? sourcePathManager.pathUrl : shortcuts.documents

        onAccepted: { sourcePathManager.onUpdateEditText(folder) }
    }
}
