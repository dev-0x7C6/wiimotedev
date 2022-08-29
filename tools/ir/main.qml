import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

ApplicationWindow {
	visible: true
	width: 640
	height: 480
    title: qsTr("wiimotedev: ir monitor")

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Cursor {
        dx: cursor.cursor.x
        dy: cursor.cursor.y
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
