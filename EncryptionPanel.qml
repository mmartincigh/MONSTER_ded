import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    border.color: "red"

    width: childrenRect.width
    height: childrenRect.height

    Button {
        id: encryptFilesButton

        anchors.left: parent.left
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr("encrypt files")
        enabled: encryptionManager.isEnabled && (processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed)

        onClicked: {
            sourceManager.onEncryptFiles();
            secureManager.onEncryptFiles();
            destinationManager.onEncryptFiles();
            encryptionManager.onEncryptFiles();
        }
    }

    ProgressBar {
        id: progressBar

        anchors.top: parent.top
        anchors.left: encryptFilesButton.right
        anchors.leftMargin: 20
        anchors.right: encryptionManagerStatus.left
        anchors.rightMargin: 20

        value: encryptionManager.progress
    }

    Text {
        id: encryptionManagerStatus

        width: 50

        anchors.right: parent.right
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr(processManager.stateDescription)
    }
}
