import QtQuick 2.5

Rectangle {
    id: main

    SourcePathPanel {
        id: sourcePathPanel

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    Row {
        id: encryptionRow

        anchors.top: sourcePathPanel.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 20

        EncryptionPanel {
            id: encryptionPanel
        }

        SourceFilesPanel {
            id: sourceFilesPanel
        }
    }

    SecurePathPanel {
        id: securePathPanel

        anchors.top: encryptionRow.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    Row {
        id: decryptionRow

        anchors.top: securePathPanel.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 20

        DecryptionPanel {
            id: decryptionPanel
        }

        SecureFilesPanel {
            id: secureFilesPanel
        }
    }

    DestinationPathPanel {
        id: destinationPathPanel

        anchors.top: decryptionRow.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    ProgressPanel {
        id: progressPanel

        anchors.top: destinationPathPanel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    ProcessPanel {
        id: processPanel

        anchors.top: progressPanel.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
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
