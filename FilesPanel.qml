import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Item {
    id: main

    height: reloadVideoFilesButton.height

    Text {
        id: videoFilesCountLabel

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadVideoFilesButton.verticalCenter

        text: qsTr("video files count:")
    }

    Text {
        id: videoFilesCount

        anchors.left: videoFilesCountLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadVideoFilesButton.verticalCenter

        text: fileManager.videoFilesCount
    }

    Button {
        id: reloadVideoFilesButton

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: videoFilesCountLabel.right
        anchors.leftMargin: 80

        text: qsTr("reload")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Idle || encryptionManager.state === Enums.Stopped || encryptionManager.state === Enums.Completed)

        onClicked: { fileManager.onReloadVideoFiles() }
    }

    Text {
        id: overwriteOutputFilesLabel

        anchors.left: reloadVideoFilesButton.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadVideoFilesButton.verticalCenter

        text: qsTr("overwrite output files")
    }

    CheckBox {
        id: overwriteOutputFiles

        anchors.left: overwriteOutputFilesLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: reloadVideoFilesButton.verticalCenter

        checked: fileManager.overwriteOutputFiles
        enabled: sourceManager.state === Enums.Idle || sourceManager.state === Enums.Stopped || sourceManager.state === Enums.Completed

        Binding {
            target: fileManager
            property: "overwriteOutputFiles"
            value: overwriteOutputFiles.checked
        }
    }
}
