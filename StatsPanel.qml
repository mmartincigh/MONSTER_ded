import QtQuick 2.5

Rectangle {
    id: main

    width: childrenRect.width
    height: childrenRect.height

    Text {
        id: processBytesLabel

        anchors.top: parent.top
        anchors.left: parent.left

        text: qsTr("processed bytes:")
    }

    Text {
        id: processBytes

        anchors.top: parent.top
        anchors.left: processBytesLabel.right
        anchors.leftMargin: 20

        text: statsManager.processedBytes + " [" + statsManager.processedBytesString + "] / " + statsManager.bytesToProcess + " [" + statsManager.bytesToProcessString + "]"
    }

    Text {
        id: errorsLabel

        anchors.top: processBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left

        text: qsTr("errors:")
    }

    Text {
        id: errors

        anchors.top: processBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: skipped.left

        text: statsManager.errors
        color: statsManager.errors > 0 ? "red" : "black"
    }

    Text {
        id: warningsLabel

        anchors.top: processBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 140

        text: qsTr("warnings:")
    }

    Text {
        id: warnings

        anchors.top: processBytesLabel.bottom
        anchors.topMargin: 20
        anchors.left: overwritten.left

        text: statsManager.warnings
        color: statsManager.warnings > 0 ? "orange" : "black"
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

        text: statsManager.skipped
        color: statsManager.skipped > 0 ? "blue" : "black"
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

        text: statsManager.overwritten
        color: statsManager.overwritten > 0 ? "blue" : "black"
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

        text: statsManager.processed
        color: statsManager.processed > 0 ? "green" : "black"
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

        text: statsManager.currentInputFile
        clip: true
    }
}
