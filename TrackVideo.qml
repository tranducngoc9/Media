import QtQuick 2.0

Item {
    width: 350
    height: 50
    property alias totalTime: totalTime.text
    property int totalTimeValue: 0
    property alias elapsedTime: elapsedTime.text
    property double timePercent: 0
    Rectangle {
        id: track
        width: parent.width
        height: parent.height
        color: "black"

        Rectangle {
            id: controlBar
            width: parent.width
            height: 50
            color: "#333"
            anchors.bottom: parent.bottom

            Text {
                id: elapsedTime
                text:"00:00"
                color: "white"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
            }

            Rectangle {
                id: progressBar
                width: parent.width - 100
                height: 5
                color: "gray"
                anchors.left: elapsedTime.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                Rectangle {
                    width:  progressBar.width * timePercent //posionTime.x - elapsedTime.width - 10
                    height: parent.height
                    color: "white"
                }
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    onClicked: (mouse) => {
                                   var clickX = mouse.x;  // Get X position from mouse and start from MouseArea
                                   var time = Math.floor(clickX / progressBar.width * totalTimeValue * 1000);

                                   ControllVideo.writeCommand("time=" + time);
                               }
                }
            }

            Text {
                id: totalTime
                text: "99:99"
                color: "white"
                anchors.left: progressBar.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
            }
        }

        Rectangle {
            id: posionTime
            x: isNaN(progressBar.width * timePercent) ? elapsedTime.width + 10 + 7 :
                                                        elapsedTime.width + 10 + 7 + progressBar.width * timePercent
            width: 15
            height: 15
            color: "red"
            radius: 15
            anchors.verticalCenter: track.verticalCenter

        }

    }
}
