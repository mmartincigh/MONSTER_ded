import QtQuick 2.0

Item {
    id: main

    QtObject {
        id: d

        property bool restartRequired: false
    }

    signal hidden()

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: message
                opacity: 0
            }
            PropertyChanges {
                target: mouseArea
                enabled: false
            }
        },
        State {
            name: "success"
            PropertyChanges {
                target: messageText
                color: "green"
            }
            PropertyChanges {
                target: message
                opacity: 1
                color: "green"
            }
            PropertyChanges {
                target: mouseArea
                enabled: true
            }
        },
        State {
            name: "warning"
            PropertyChanges {
                target: messageText
                color: "orange"
            }
            PropertyChanges {
                target: message
                opacity: 1
                color: "orange"
            }
            PropertyChanges {
                target: mouseArea
                enabled: true
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: messageText
                color: "red"
            }
            PropertyChanges {
                target: message
                opacity: 1
            }
            PropertyChanges {
                target: headerText
                text: qsTr("error")
            }
            PropertyChanges {
                target: dismissText
                text: qsTr("click to dismiss")
            }
            PropertyChanges {
                target: mouseArea
                enabled: true
            }
        }
    ]
    transitions: [
        Transition {
            to: "hidden"
            ScriptAction {
                script: {
                    timer.stop();
                    hideAnimation.start();
                    main.hidden();
                }
            }
        },
        Transition {
            to: "success"
            ScriptAction {
                script: { timer.start() }
            }
        },
        Transition {
            to: "warning"
            ScriptAction {
                script: { timer.start() }
            }
        },
        Transition {
            to: "error"
            ScriptAction {
                script: {
                    timer.stop();
                    showAnimation.start();
                }
            }
        }
    ]

    Connections {
        target: encryptionManager
        /*onSuccessMessage: {
            if (main.state == "warning" || main.state == "error") { return }
            headerText.text = qsTr("success");
            messageText.text = qsTr(successMessage);
            dismissText.text = qsTr("wait or click to dismiss");
            main.state = "success";
        }*/
        onWarningMessage: {
            if (main.state == "error") { return }
            headerText.text = qsTr("warning");
            messageText.text = qsTr(warningMessage);
            dismissText.text = qsTr("wait or click to dismiss");
            main.state = "warning";
        }
        onErrorMessage: {
            messageText.text = qsTr(errorMessage);
            //dismissText.text = qsTr(restartRequired ? "restart the application" : "click to dismiss")
            //d.restartRequired = restartRequired
            main.state = "error";
        }
    }

    Timer {
        id: timer

        interval: 3000

        onTriggered: { main.state = "hidden" }
    }

    Rectangle {
        id: header

        width: headerText.width > 0 ? headerText.width + 20 : 0
        height: headerText.width > 0 ? headerText.height + 10 : 0
        anchors.centerIn: message

        opacity: message.opacity
        visible: opacity > 0
        border.color: messageText.color

        Text {
            id: headerText

            anchors.centerIn: parent

            color: messageText.color
            font.family: "Segoe UI"
            font.pointSize: 12
        }
    }

    Rectangle {
        id: dismiss

        width: dismissText.width > 0 ? dismissText.width + 20 : 0
        height: dismissText.width > 0 ? dismissText.height + 10 : 0
        anchors.centerIn: message

        opacity: message.opacity
        visible: opacity > 0
        border.color: messageText.color

        Text {
            id: dismissText

            anchors.centerIn: parent

            color: messageText.color
            font.family: "Segoe UI"
            font.pointSize: 12
        }
    }

    Rectangle {
        id: message

        width: messageText.width > 0 ? messageText.width + 40 : 0
        height: messageText.height > 0 ? messageText.height + 20 : 0
        anchors.centerIn: parent

        visible: opacity > 0
        border.color: messageText.color
        border.width: 3

        /*Behavior on color {
            ColorAnimation { duration: 250 }
        }*/
        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }

        Text {
            id: messageText

            width: 300
            anchors.centerIn: parent

            font.family: "Segoe UI"
            font.bold: true
            font.pointSize: 14
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: false

        onClicked: {
            if (d.restartRequired) { return }
            main.state = "hidden";
        }
    }

    ParallelAnimation {
        id: showAnimation

        NumberAnimation {
            target: message
            property: "height"
            from: messageText.height
            to: messageText.height + 20
            duration: 250
        }
        NumberAnimation {
            target: header
            property: "anchors.verticalCenterOffset"
            from: 0
            to: -(messageText.height + 20 + header.height) / 2 - 10
            duration: 250
        }
        NumberAnimation {
            target: dismiss
            property: "anchors.verticalCenterOffset"
            from: 0
            to: (messageText.height + 20 + header.height) / 2 + 10
            duration: 250
        }
    }

    ParallelAnimation {
        id: hideAnimation

        NumberAnimation {
            target: message
            property: "height"
            to: messageText.height
            duration: 250
        }
        NumberAnimation {
            target: header
            property: "anchors.verticalCenterOffset"
            to: 0
            duration: 250
        }
        NumberAnimation {
            target: dismiss
            property: "anchors.verticalCenterOffset"
            to: 0
            duration: 250
        }
    }
}
