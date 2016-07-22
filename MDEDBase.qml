import QtQuick 2.3

Rectangle {
    id: main

    focus: true

    Keys.onEscapePressed: { Qt.quit() }
    Keys.onLeftPressed: { panelArea.moveToLeft() }
    Keys.onRightPressed: { panelArea.moveToRight() }

    MDEDPanelArea {
        id: panelArea

        anchors.fill: parent
    }
}
