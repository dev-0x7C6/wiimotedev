import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

ApplicationWindow {
	visible: true
    width: 1024
    height: 768
    title: qsTr("wiimotedev: ir monitor")

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image {
        id: image
        source: "qrc:/bg.png"
        anchors.fill: parent
        anchors.margins: parent.width * 0.05
        smooth: true
        antialiasing: true
        fillMode: Image.PreserveAspectFit
        opacity: 1

        NumberAnimation on opacity {
            running: cursor.cursor.visible
            to: 0.2
            duration: 400
        }

        NumberAnimation on opacity {
            running: !cursor.cursor.visible
            to: 1
            duration: 400
        }

        rotation: -cursor.cursor.roll
    }

    Cursor {
        dx: cursor.cursor.x
        dy: cursor.cursor.y
        yaw: cursor.cursor.yaw
        roll: cursor.cursor.roll
        pitch: cursor.cursor.pitch
        distance: cursor.cursor.distance
        vc: cursor.cursor.visible
    }

    IrPoint {
        num: 1
        dx: cursor.ir.x1
        dy: cursor.ir.y1
        dv: cursor.ir.v1
        color: "yellow"
    }

    IrPoint {
        num: 2
        dx: cursor.ir.x2
        dy: cursor.ir.y2
        dv: cursor.ir.v2
        color: "yellow"
    }

    IrPoint {
        num: 3
        dx: cursor.ir.x3
        dy: cursor.ir.y3
        dv: cursor.ir.v3
        color: "yellow"
    }

    IrPoint {
        num: 4
        dx: cursor.ir.x4
        dy: cursor.ir.y4
        dv: cursor.ir.v4
        color: "yellow"
    }

}
