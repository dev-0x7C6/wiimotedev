import QtQuick 2.15

Item {
    property var model;

    IrPoint {
        num: 1
        dx: model.x1
        dy: model.y1
        dv: model.v1
    }

    IrPoint {
        num: 2
        dx: model.x2
        dy: model.y2
        dv: model.v2
    }

    IrPoint {
        num: 3
        dx: model.x3
        dy: model.y3
        dv: model.v3
    }

    IrPoint {
        num: 4
        dx: model.x4
        dy: model.y4
        dv: model.v4
    }

    Cursor {
        dx: model.x
        dy: model.y
        yaw: model.yaw
        roll: model.roll
        pitch: model.pitch
        distance: model.distance
        vc: model.visible
        press: model.press
        cid: model.id
    }
}
