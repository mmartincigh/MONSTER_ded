import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    border.color: "red"

    width: childrenRect.width
    height: childrenRect.height

    Button {
        id: encryptButton

        text: qsTr("encrypt")
        enabled: encryptionManager.isEnabled && (processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed)

        onClicked: {
            sourcePathManager.onProcess();
            securePathManager.onProcess();
            destinationPathManager.onProcess();
            encryptionManager.onProcess();
        }
    }
}
