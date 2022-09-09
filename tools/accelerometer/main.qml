import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

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
