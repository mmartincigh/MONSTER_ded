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
        target: secureManager
        onEditTextChanged: { securePathComboBox.editText = editText }
    }

    Text {
        id: securePathLabel

        anchors.left: parent.left
        anchors.verticalCenter: securePathComboBox.verticalCenter

        text: qsTr("secure path")
    }

    ComboBox {
        id: securePathComboBox

        height: 25
        anchors.top: parent.top
        anchors.left: securePathLabel.right
        anchors.leftMargin: 20
        anchors.right: securePathBrowseButton.left
        anchors.rightMargin: 20

        model: secureManager.securePathModel
        editable: true
        enabled: main.enabled

        onEditTextChanged: { secureManager.onUpdateEditText(editText) }

        Text {
            id: securePathOverlayText

            anchors.centerIn: parent

            text: qsTr("enter or select the secure path")
            color: "grey"
            opacity: parent.activeFocus || parent.editText.length > 0 ? 0 : 1

            Behavior on opacity {
                NumberAnimation { duration: 125 }
            }
        }
    }

    Button {
        id: securePathBrowseButton

        anchors.right: parent.right
        anchors.verticalCenter: securePathComboBox.verticalCenter

        text: qsTr("browse")
        enabled: main.enabled

        onClicked: { fileDialog.open() }
    }

    FileDialog {
        id: fileDialog

        title: qsTr("Select a folder")
        selectFolder: true
        folder: secureManager.isSecurePathUrlValid ? secureManager.securePathUrl : shortcuts.documents

        onAccepted: { secureManager.onUpdateEditText(folder) }
    }
}
