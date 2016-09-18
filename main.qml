import QtQuick 2.7
import QtQuick.Window 2.2
import DrawItems 1.0

Window {
    id: window1
    visible: true
    width: 1080
    height: 1920
    title: qsTr("RakVideo Live")

    Text {
        text: qsTr("RakVideo Live")
        anchors.verticalCenterOffset: -611
        anchors.horizontalCenterOffset: 1
        anchors.centerIn: parent
    }



    Rectangle {
        id: rectangle1
        radius: 0
        border.width: 10
        border.color: "#9a76f2"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 1330

        MouseArea{
            x: 0
            y: 1330
            anchors.fill: parent
            onClicked:{
                console.warn("fuck!");
            }
        }

        Text {
            id: text1
            text: qsTr("Button Test Area")
            font.pixelSize: 50
            font.family: "Tahoma"
            style: Text.Normal
            font.bold: true
            textFormat: Text.AutoText
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
        }
    }

    Rectangle {
        id: rectangle2
        width: 660
        height: 500
        border.color: "#fae6a0"
        border.width: 4
        anchors.verticalCenterOffset: -250
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        QDrawImageItem {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            id: drawImageItem

            Component.onCompleted: {
                signal_class.SignalImage.connect(function(in_image) {
                    drawImageItem.changeImage(in_image);
                })
            }
        }

        MouseArea {
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: drawImageItem
            onClicked: {
                rak.callRakJava("message");
            }
        }
    }
}
