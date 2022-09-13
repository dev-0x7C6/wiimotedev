import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
	visible: true
    width: 1366
    height: 768
    title: qsTr("wiimotedev: ir monitor")

    Rectangle {
        anchors.fill: parent
        color: "black"

        Image {
            source: "qrc:/assets/background.png"
            anchors.fill: parent
            anchors.margins: parent.width * 0.05
            smooth: true
            antialiasing: true
            fillMode: Image.PreserveAspectFit
            opacity: 0.2
        }
    }

    property var model: cursor.model
    property var cursor1
    property var cursor2
    property var cursor3
    property var cursor4

    onModelChanged: {
        if (cursor1 !== model[0])
            cursor1 = model[0]
        if (cursor2 !== model[1])
            cursor2 = model[1]
        if (cursor3 !== model[2])
            cursor3 = model[2]
        if (cursor4 !== model[3])
            cursor4 = model[3]
    }

    CursorScene {
        anchors.fill: parent
        model: cursor4
    }

    CursorScene {
        anchors.fill: parent
        model: cursor3
    }

    CursorScene {
        anchors.fill: parent
        model: cursor2
    }

    CursorScene {
        anchors.fill: parent
        model: cursor1
    }
}
