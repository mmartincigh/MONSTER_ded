import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    width: childrenRect.width
    height: childrenRect.height

    Button {
        id: pauseButton

        anchors.top: top.bottom
        anchors.left: parent.left

        text: qsTr("pause")
        enabled: processManager.isEnabled && (processManager.state === Enums.ProcessState_Working)

        onClicked: { processManager.onPause() }
    }

    Button {
        id: resumeButton

        anchors.top: top.bottom
        anchors.left: pauseButton.right
        anchors.leftMargin: 20

        text: qsTr("resume")
        enabled: processManager.isEnabled && (processManager.state === Enums.ProcessState_Paused)

        onClicked: { processManager.onResume() }
    }

    Button {
        id: stopButton

        anchors.top: top.bottom
        anchors.left: resumeButton.right
        anchors.leftMargin: 20

        text: qsTr("stop")
        enabled: processManager.isEnabled && (processManager.state === Enums.ProcessState_Paused || processManager.state === Enums.ProcessState_Working)

        onClicked: { processManager.onStop() }
    }
}
