import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    border.color: "red"

    width: childrenRect.width
    height: childrenRect.height

    Button {
        id: decryptFilesButton

        anchors.left: parent.left
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr("decrypt files")
        enabled: decryptionManager.isEnabled && (processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed)

        onClicked: {
            sourceManager.onDecryptFiles();
            secureManager.onDecryptFiles();
            destinationManager.onDecryptFiles();
            decryptionManager.onDecryptFiles();
        }
    }

    ProgressBar {
        id: progressBar

        anchors.top: parent.top
        anchors.left: decryptFilesButton.right
        anchors.leftMargin: 20
        anchors.right: decryptionManagerStatus.left
        anchors.rightMargin: 20

        value: decryptionManager.progress
    }

    Text {
        id: decryptionManagerStatus

        width: 50

        anchors.right: parent.right
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr(processManager.stateDescription)
    }
}
