import QtQuick 2.15

Item {
    property int dx : 0
    property int dy : 0
    property double angle : 0
    x: -dx + parent.width / 2
    y: -dy + parent.height / 2

    Image {
        id: image
        source: "qrc:/cursor.png"
        anchors.left: parent.left
        anchors.top: parent.bottom
        fillMode: Image.PreserveAspectFit
        width: 50
        height: 50
        rotation: angle
    }

    Text {
        anchors.left: image.left
        anchors.top: image.bottom
        color: "white"
        text: dx + "x" + dy
        font.pointSize: 8
    }
}
