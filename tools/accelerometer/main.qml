import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
	visible: true
    width: 1024
    height: 768
    title: qsTr("wiimotedev: accelerometer monitor")

    Rectangle {
        anchors.fill: parent
        color: "black"
    }


    Text {
        anchors.centerIn: parent
        rotation: accel.data.roll
        text: "roll: " + accel.data.roll
        color: "yellow"
    }

    Text {
        anchors.centerIn: parent
        rotation: accel.data.pitch
        text: "pitch: " + accel.data.pitch
        color: "yellow"
    }
}
