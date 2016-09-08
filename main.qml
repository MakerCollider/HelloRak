import QtQuick 2.7
import QtQuick.Window 2.2
import DrawItems 1.0

Window {
    visible: true
    width: 1080
    height: 1920
    title: qsTr("RakVideo Live")

    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            rak.callRakJava("message");
        }
    }

    Text {
        text: qsTr("RakVideo Live")
        anchors.verticalCenterOffset: -611
        anchors.horizontalCenterOffset: 1
        anchors.centerIn: parent
    }

    QDrawImageItem {
        width: 640
        height: 480
        id: drawImageItem
        x: 221
        y: 676

        Component.onCompleted: {
            signal_class.SignalImage.connect(function(in_image) {
                drawImageItem.changeImage(in_image);
            })
        }
    }
}
