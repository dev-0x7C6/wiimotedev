import QtQuick 2.15

Item {
    property double dx : 0
    property double dy : 0
    property double yaw : 0
    property double roll : 0
    property double pitch : 0
    property double distance : 1.0
    x: parent.width / 2 + dx
    y: parent.height / 2 + dy

    Image {
        id: image
        source: "qrc:/cursor.png"
        anchors.left: parent.left
        anchors.top: parent.bottom
        fillMode: Image.PreserveAspectFit
        width: 50 * distance / 75
        height: 50 * distance / 75
        rotation: roll

        Text {
            property int ix: dx
            property int iy: dy
            property int d: distance

            anchors.left: image.left
            anchors.top: image.bottom
            color: "white"
            text: ix + "x" + iy + "\n" + d + "cm"
            font.pointSize: 14 * (distance / 75)
        }
    }
}
