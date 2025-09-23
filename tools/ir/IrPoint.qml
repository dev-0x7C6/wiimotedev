import QtQuick

Item {
    property int dx : 0
    property int dy : 0
    property int num : 0
    property color color: "yellow"

    x: parent.width / 1024.0 * dx;
    y: parent.height / 768.0 * dy;
    width: 0
    height: 0

    Rectangle {
        id: dot
        anchors.centerIn: parent
        radius: 10
        width: 10
        height: 10
        border.width: 1
        border.color: "yellow"
        color: parent.color
    }

    Text {
        anchors.top: dot.top
        anchors.left: dot.right
        leftPadding: 8
        color: "white"
        text: num + ": " + dx + "x" + dy
        font.pointSize: 8
    }
}
