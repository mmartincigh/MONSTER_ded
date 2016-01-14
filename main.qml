import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: main

    signal completed()

    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2
    width: 650
    height: 380

    title: "MONSTER_ded"
    flags: windowManager.windowFlags
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height

    Component.onCompleted: {
        completed();
        visible = true;
    }

    Base {
        anchors.fill: parent

        focus: true
    }
}
