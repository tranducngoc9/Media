import QtQuick 2.0

Rectangle {
    id: button
    width: 80
    height: 30
    color: "lightgray"
    radius: 5
    border.color: "black"

    property alias text: buttonText.text
    signal clicked

    Text {
        id: buttonText
        anchors.centerIn: parent
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: button.clicked()
        onPressed: button.color = "gray"
        onReleased: button.color = "lightgray"
    }
}
