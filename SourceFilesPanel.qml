import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    border.color: "red"

    width: childrenRect.width
    height: childrenRect.height

    Text {
        id: inputFilesCountLabel

        anchors.left: parent.left
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        text: qsTr("source files count:")
    }

    Text {
        id: inputFilesCount

        anchors.left: inputFilesCountLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        text: sourceFileManager.inputFilesCount
    }

    Button {
        id: reloadInputFilesButton

        anchors.top: parent.top
        anchors.left: inputFilesCountLabel.right
        anchors.leftMargin: 80

        text: qsTr("reload")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.ProcessState_Idle || encryptionManager.state === Enums.ProcessState_Stopped || encryptionManager.state === Enums.ProcessState_Completed)

        onClicked: { sourceFileManager.onReloadInputFiles() }
    }

    Text {
        id: overwriteOutputFilesLabel

        anchors.left: reloadInputFilesButton.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        text: qsTr("overwrite output files")
    }

    CheckBox {
        id: overwriteOutputFiles

        anchors.left: overwriteOutputFilesLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        checked: sourceFileManager.overwriteOutputFiles
        enabled: encryptionManager.state === Enums.ProcessState_Idle || encryptionManager.state === Enums.ProcessState_Stopped || encryptionManager.state === Enums.ProcessState_Completed

        Binding {
            target: sourceFileManager
            property: "overwriteOutputFiles"
            value: overwriteOutputFiles.checked
        }
    }
}
