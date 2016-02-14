import QtQuick 2.5

Rectangle {
    id: main

    border.color: "red"

    width: childrenRect.width
    height: childrenRect.height

    Text {
        id: encryptedBytesLabel

        anchors.top: parent.top
        anchors.left: parent.left

        text: qsTr("encrypted bytes:")
    }

    Text {
        id: encryptedBytes

        anchors.top: parent.top
        anchors.left: encryptedBytesLabel.right
        anchors.leftMargin: 20

        text: encryptionManager.encryptedBytes + " [" + encryptionManager.encryptedBytesString + "] / " + encryptionManager.bytesToEncrypt + " [" + encryptionManager.bytesToEncryptString + "]"
    }

    Text {
        id: errorsLabel

        anchors.top: encryptedBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left

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

        text: qsTr("current input file:")
    }

    Text {
        id: currentInputFile

        anchors.top: skippedLabel.bottom
        anchors.topMargin: 20
        anchors.left: currentInputFileLabel.right
        anchors.leftMargin: 20
        anchors.right: parent.right

        text: encryptionManager.currentInputFile
        clip: true
    }
}
