import QtQuick 2.7

Item {
    property double pressure;
    property double all_pressure;

    Rectangle {
        anchors.fill: parent
        radius: 20
        color: "#50a0ff";
        opacity: 0.8 * parent.pressure / parent.all_pressure;

    }

    Text {
        id: name
        text: pressure;
        anchors.centerIn: parent
        font.pixelSize: 10 + parent.pressure / 100
    }
}
