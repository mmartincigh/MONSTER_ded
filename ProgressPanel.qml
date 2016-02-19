import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    width: childrenRect.width
    height: childrenRect.height

    Text {
        id: processType

        width: 50

        anchors.left: parent.left
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr(processManager.typeDescription)
    }

    ProgressBar {
        id: progressBar

        anchors.top: parent.top
        anchors.left: processType.right
        anchors.leftMargin: 20
        anchors.right: processStatus.left
        anchors.rightMargin: 20

        value: processManager.progress

        Text {
            id: progress

            anchors.centerIn: parent

            opacity: (processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed) ? 0 : 1
            text: encryptionManager.progressString

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }
        }
    }

    Text {
        id: processStatus

        width: 50

        anchors.right: parent.right
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr(processManager.stateDescription)
    }
}
