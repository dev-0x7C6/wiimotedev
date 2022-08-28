import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

ApplicationWindow {
	visible: true
	width: 640
	height: 480
    title: qsTr("wiimotedev: balance board monitor")

    Rectangle {
        anchors.fill: parent
        color: "#404040"
    }

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "balanceboard.png"

        GridLayout {
            anchors.centerIn: parent
            width: parent.paintedWidth * 0.86
            height: parent.paintedHeight * 0.80
            columns: 2
            rows: 2
            property double sum: (balanceboard.tl + balanceboard.tr + balanceboard.bl + balanceboard.br);

            PressArea {
                pressure: balanceboard.tl;
                all_pressure: parent.sum;
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            PressArea {
                pressure: balanceboard.tr;
                all_pressure: parent.sum;
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            PressArea {
                pressure: balanceboard.bl;
                all_pressure: parent.sum;
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            PressArea {
                pressure: balanceboard.br;
                all_pressure: parent.sum;
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
