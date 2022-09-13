import QtQuick 2.15

Item {
    property int dx : 0
    property int dy : 0
    property int num : 0
    property bool dv: false
    property string color: "yellow"

    x: parent.width / 1024.0 * dx;
    y: parent.height / 768.0 * dy;
    visible: dv

    Rectangle {
        radius: 8
        width: 8
        height: 8
        x: -(width / 2)
        y: -(height / 2)
        color: parent.color
    }

    Text {
        anchors.left: parent.left
        anchors.topMargin: 6
        anchors.leftMargin: 6
        color: "white"
        text: "ir: " + num + ": " + dx + "x" + dy
        font.pointSize: 8
    }
}
