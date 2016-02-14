import QtQuick 2.5

Rectangle {
    id: main

    SourcePanel {
        id: sourcePanel

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    SecurePanel {
        id: securePanel

        anchors.top: sourcePanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    DestinationPanel {
        id: destinationPanel

        anchors.top: securePanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    FilesPanel {
        id: filesPanel

        anchors.top: destinationPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    EncryptionPanel {
        id: encryptionPanel

        anchors.top: filesPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    DecryptionPanel {
        id: decryptionPanel

        anchors.top: encryptionPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    ProcessPanel {
        id: processPanel

        anchors.top: decryptionPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    StatsPanel {
        id: statsPanel

        anchors.top: processPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    Text {
        id: version

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        text: qsTr("version ") + application.applicationVersion
    }
}
