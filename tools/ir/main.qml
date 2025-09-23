import QtQuick
import QtQuick.Controls
import Wiimotedev

ApplicationWindow {
    id: root
	visible: true
    width: 1366
    height: 768
    title: qsTr("wiimotedev: ir monitor")

    Rectangle {
        id: area
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

    VirtualCursorModel { id: vcModel; }
    InfraredModel { id: irModel; }

    function get_cursor_color(id) {
        switch (id % 5) {
            case 0: return "#ffffff"
            case 1: return "#ffb300"
            case 2: return "#00a65f"
            case 3: return "#ff4033"
            case 4: return "#008cff"
        }

        return "#ffb300"
    }

    Repeater {
        anchors.fill: area
        model: irModel

        IrPoint {
            num: model.infrared["num"]
            dx: model.infrared["x"]
            dy: model.infrared["y"]
            visible: model.infrared["visible"]
            color: get_cursor_color(model.infrared["id"])
        }
    }

    Repeater {
        anchors.fill: area
        model: vcModel

        Cursor {
            cid: model.cursor["id"]
            dx: model.cursor["x"]
            dy: model.cursor["y"]
            vc: model.cursor["visible"]
            yaw: model.cursor["yaw"]
            roll: model.cursor["roll"]
            pitch: model.cursor["pitch"]
            distance: model.cursor["distance"]
            press: model.cursor["press"]

            z: distance
            color: get_cursor_color(model.cursor["id"])
        }
    }
}
