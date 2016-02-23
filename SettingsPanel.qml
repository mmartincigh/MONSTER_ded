import QtQuick 2.5
import QtQuick.Controls 1.4
import MONSTER.MONSTER_ded 1.0

Rectangle {
    id: main

    property bool enabled: decryptionManager.isEnabled && (processManager.state === Enums.ProcessState_Idle || processManager.state === Enums.ProcessState_Stopped || processManager.state === Enums.ProcessState_Completed)

    width: 200 //childrenRect.width
    height: childrenRect.height

    Text {
        id: openFileAfterDecryptionLabel

        anchors.left: parent.left
        anchors.verticalCenter: openFileAfterDecryption.verticalCenter

        text: qsTr("open file after decryption")
    }

    CheckBox {
        id: openFileAfterDecryption

        anchors.left: openFileAfterDecryptionLabel.right
        anchors.leftMargin: 20

        checked: processManager.openFile
        enabled: main.enabled

        Binding {
            target: processManager
            property: "openFile"
            value: openFileAfterDecryption.checked
        }
    }

    Text {
        id: info

        anchors.top: openFileAfterDecryption.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.right: parent.right

        text: qsTr("<b><font color=\"blue\">info</font></b> it applies only to individual files opened using the application directly (for example, by double-clicking on them)")
        textFormat: Text.RichText
        wrapMode: Text.WordWrap
    }
}
