import QtQuick 2.5
import QtGraphicalEffects 1.0

Item {
    id: main

    Keys.onEscapePressed: { Qt.quit() }

    Connections {
        target: encryptionManager
        /*onSuccessMessage: {
            blurEngline.blur = true;
        }*/
        onWarningMessage: {
            blurEngline.blur = true;
        }
        onErrorMessage: {
            blurEngline.blur = true;
        }
    }

    PanelArea {
        id: panelArea

        anchors.fill: parent
    }

    FastBlur {
        id: blurEngline

        property bool blur: false

        anchors.fill: parent

        source: panelArea
        radius: blur ? 64 : 0

        Behavior on radius {
            NumberAnimation { duration: 250 }
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        enabled: blurEngline.blur
        hoverEnabled: blurEngline.blur
        propagateComposedEvents: !blurEngline.blur

        /*onClicked: {
            blurEngline.blur = false;
        }*/
    }

    Messages {
        id: messages

        anchors.fill: parent

        state: "hidden"

        onHidden: { blurEngline.blur = false }
    }
}
