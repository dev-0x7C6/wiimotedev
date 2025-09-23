import QtQuick

Item {
    property var provider: model
    Cursor {
        cid: provider.cursor["id"]
        dx: provider.cursor["x"]
        dy: provider.cursor["y"]
        vc: provider.cursor["visible"]
        yaw: provider.cursor["yaw"]
        roll: provider.cursor["roll"]
        pitch: provider.cursor["pitch"]
        distance: provider.cursor["distance"]
    }
}
