import QtQuick 2.15

Rectangle {
    property int dx : 0
    property int dy : 0
    property int num : 0
    property bool dv: false
    x: parent.width / 1024.0 * dx;
    y: parent.height / 768.0 * dy;
    visible: dv

    Text {
        anchors.left: parent.left
        anchors.top: parent.bottom
        color: "white"
        text: "ir: " + num + ": " + dx + "x" + dy
        font.pointSize: 8
    }

    radius: 10
    width: 10
    height: 10
}
