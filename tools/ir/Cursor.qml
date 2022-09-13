import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    property double dx : 0
    property double dy : 0
    property double yaw : 0
    property double roll : 0
    property double pitch : 0
    property double distance : 1.0
    property bool vc : false
    property bool press : false
    property int cid : 0.0
    x: parent.width / 2 + dx
    y: parent.height / 2 + dy


    NumberAnimation on opacity {
        running: vc
        to: 1.0
        duration: 150
    }

    NumberAnimation on opacity {
        running: !vc
        to: 0.0
        duration: 400
    }

    function get_cursor_asset(is_grabbing, id) {
        if (id < 1 || id > 4)
            id = 0;

        var catalog = is_grabbing ? "grabbing" : "pointing";
        return "qrc:/assets/cursor/" + catalog + "/0" + id + ".png"
    }

    function get_cursor_color(id) {
        if (id < 1 || id > 4)
            id = 0;

        switch (id) {
            case 1: return "#008cff"
            case 2: return "#ff4033"
            case 3: return "#00a65f"
            case 4: return "#ffb300"
        }

        return "#008cff"
    }

    function get_cursor_asset_bg(is_grabbing) {
        var catalog = is_grabbing ? "grabbing" : "pointing";
        return "qrc:/assets/cursor/" + catalog + "/background.png"
    }

    function get_cursor_asset_shadow(is_pointing) {
        var catalog = is_grabbing ? "grabbing" : "pointing";
        return "qrc:/assets/cursor/" + catalog + "/shadow.png"
    }


    Item {
        readonly property double ratio: distance / 75.0
        id: layers

        anchors.left: parent.left
        anchors.top: parent.bottom
        width: 75 * ratio
        height: 75 * ratio

        transform: Rotation {
            origin.x: 49 * ratio;
            origin.y: 5 * ratio;
            angle: roll
        }

        Image {
            id: shadow
            anchors.fill: layers
            source: get_cursor_asset_shadow(press, cid)
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: background
            anchors.fill: layers
            source: get_cursor_asset_bg(press, cid)
            fillMode: Image.PreserveAspectFit
            opacity: 0.2
        }

        Image {
            id: cursor
            anchors.fill: layers
            source: get_cursor_asset(press, cid)
            fillMode: Image.PreserveAspectFit
            visible: false
        }

        ColorOverlay {
            anchors.fill: layers
            source: cursor
            color: get_cursor_color(cid)
        }
    }
}
