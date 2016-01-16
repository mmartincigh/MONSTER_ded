import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Item {
    id: main

    height: reloadInputFilesButton.height

    Text {
        id: inputFilesCountLabel

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        text: qsTr("input files count:")
    }

    Text {
        id: inputFilesCount

        anchors.left: inputFilesCountLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadInputFilesButton.verticalCenter

        text: fileManager.inputFilesCount
    }

    Button {
        id: reloadInputFilesButton

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: inputFilesCountLabel.right
        anchors.leftMargin: 80

        text: qsTr("reload")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Idle || encryptionManager.state === Enums.Stopped || encryptionManager.state === Enums.Completed)

        onClicked: { fileManager.onReloadInputFiles() }
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

        checked: fileManager.overwriteOutputFiles
        enabled: sourceManager.state === Enums.Idle || sourceManager.state === Enums.Stopped || sourceManager.state === Enums.Completed

        Binding {
            target: fileManager
            property: "overwriteOutputFiles"
            value: overwriteOutputFiles.checked
        }
    }
}
