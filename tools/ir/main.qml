import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
	visible: true
    width: 1024
    height: 768
    title: qsTr("wiimotedev: ir monitor")

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    property var model: cursor.model

    Image {
        id: image
        source: "qrc:/bg.png"
        anchors.fill: parent
        anchors.margins: parent.width * 0.05
        smooth: true
        antialiasing: true
        fillMode: Image.PreserveAspectFit
        opacity: 0.3
    }

    Repeater {
        anchors.fill: root

        model: root.model
        delegate: Item {
            x: 0
            y: 0
            width: root.width
            height: root.height

            Cursor {
                dx: model.modelData.x
                dy: model.modelData.y
                yaw: model.modelData.yaw
                roll: model.modelData.roll
                pitch: model.modelData.pitch
                distance: model.modelData.distance
                vc: model.modelData.visible
            }

            IrPoint {
                num: 1
                dx: model.modelData.x1
                dy: model.modelData.y1
                dv: model.modelData.v1
                color: "yellow"
            }

            IrPoint {
                num: 2
                dx: model.modelData.x2
                dy: model.modelData.y2
                dv: model.modelData.v2
                color: "yellow"
            }

            IrPoint {
                num: 3
                dx: model.modelData.x3
                dy: model.modelData.y3
                dv: model.modelData.v3
                color: "yellow"
            }

            IrPoint {
                num: 4
                dx: model.modelData.x4
                dy: model.modelData.y4
                dv: model.modelData.v4
                color: "yellow"
            }
        }
    }
}
