import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import MONSTER.MONSTER_ded 1.0

Item {
    id: main

    Button {
        id: encryptFilesButton

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr("encrypt files")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Idle || encryptionManager.state === Enums.Stopped || encryptionManager.state === Enums.Completed)

        onClicked: {
            encryptionManager.onEncryptFiles();
            destinationManager.onEncryptFiles();
            sourceManager.onEncryptFiles();
        }
    }

    ProgressBar {
        id: progressBar

        anchors.top: parent.bottom
        anchors.topMargin: 20
        anchors.left: encryptFilesButton.right
        anchors.leftMargin: 20
        anchors.right: encryptionManagerStatus.left
        anchors.rightMargin: 20

        value: encryptionManager.progress
    }

    Text {
        id: encryptionManagerStatus

        width: 50

        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: progressBar.verticalCenter

        text: qsTr(encryptionManager.stateDescription)
    }

    Button {
        id: pauseButton

        anchors.top: progressBar.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        text: qsTr("pause")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Working)

        onClicked: { encryptionManager.onPause() }
    }

    Button {
        id: resumeButton

        anchors.top: progressBar.bottom
        anchors.topMargin: 20
        anchors.left: pauseButton.right
        anchors.leftMargin: 20

        text: qsTr("resume")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Paused)

        onClicked: { encryptionManager.onResume() }
    }

    Button {
        id: stopButton

        anchors.top: progressBar.bottom
        anchors.topMargin: 20
        anchors.left: resumeButton.right
        anchors.leftMargin: 20

        text: qsTr("stop")
        enabled: encryptionManager.isEnabled && (encryptionManager.state === Enums.Paused || encryptionManager.state === Enums.Working)

        onClicked: { encryptionManager.onStop() }
    }

    Text {
        id: progressLabel

        anchors.left: stopButton.right
        anchors.leftMargin: 40
        anchors.verticalCenter: stopButton.verticalCenter

        text: qsTr("progress:")
    }

    Text {
        id: progress

        anchors.left: progressLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: stopButton.verticalCenter

        text: encryptionManager.progressString
    }

    Text {
        id: encryptedBytesLabel

        anchors.top: pauseButton.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        text: qsTr("encrypted bytes:")
    }

    Text {
        id: encryptedBytes

        anchors.top: pauseButton.bottom
        anchors.topMargin: 20
        anchors.left: encryptedBytesLabel.right
        anchors.leftMargin: 20

        text: encryptionManager.encryptedBytes + " / " + encryptionManager.bytesToEncrypt
    }

    Text {
        id: errorsLabel

        anchors.top: encryptedBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        text: qsTr("errors:")
    }

    Text {
        id: errors

        anchors.top: encryptedBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: skipped.left

        text: encryptionManager.errors
        color: encryptionManager.errors > 0 ? "red" : "black"
    }

    Text {
        id: warningsLabel

        anchors.top: encryptedBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 140

        text: qsTr("warnings:")
    }

    Text {
        id: warnings

        anchors.top: encryptedBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: overwritten.left

        text: encryptionManager.warnings
        color: encryptionManager.warnings > 0 ? "orange" : "black"
    }

    Text {
        id: skippedLabel

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        text: qsTr("skipped:")
    }

    Text {
        id: skipped

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: skippedLabel.right
        anchors.leftMargin: 20

        text: encryptionManager.skipped
        color: encryptionManager.skipped > 0 ? "blue" : "black"
    }

    Text {
        id: overwrittenLabel

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 140

        text: qsTr("overwritten:")
    }

    Text {
        id: overwritten

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: overwrittenLabel.right
        anchors.leftMargin: 20

        text: encryptionManager.overwritten
        color: encryptionManager.overwritten > 0 ? "blue" : "black"
    }

    Text {
        id: processedLabel

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: overwrittenLabel.right
        anchors.leftMargin: 80

        text: qsTr("processed:")
    }

    Text {
        id: processed

        anchors.top: errorsLabel.bottom
        anchors.topMargin: 20
        anchors.left: processedLabel.right
        anchors.leftMargin: 20

        text: encryptionManager.processed
        color: encryptionManager.processed > 0 ? "green" : "black"
    }

    Text {
        id: currentInputFileLabel

        anchors.top: skippedLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        text: qsTr("current input file:")
    }

    Text {
        id: currentInputFile

        anchors.top: skippedLabel.bottom
        anchors.topMargin: 20
        anchors.left: currentInputFileLabel.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        text: encryptionManager.currentInputFile
        clip: true
    }
}
