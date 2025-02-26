import QtQuick 2.0
import QtQuick.Window 2.0
Item {
    width: 600
    height: 450
    property int currentPage: 0
    property int pageSize: 10
    property int totalPages: 0//Math.ceil(videoModel.rowCount() / pageSize)
    property int totalTimeValue: 0
    // List video
    Rectangle {
        id:list_video
        width: parent.width
        height: 350
        color: "#121212" // Dark backgound

        ListView {
            id: listView
            width: parent.width
            height: parent.height
            model: videoModel
            delegate: Item {
                width: listView.width
                visible: index >= currentPage*10 && index < (currentPage + 1)*10
                height: index >= currentPage*10 && index < (currentPage + 1)*10 ? 35 : 0   //35  //reduce height to fit with 10 items
                Rectangle {
                    id: card
                    width: parent.width - 20
                    height: parent.height - 5
                    color: "#181818"  //  item backgound
                    radius: 8
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "#1DB954" // Green
                    border.width: 1.5
                    opacity: 0.95

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10  // Distances among texts

                        Text {
                            text: name
                            font.bold: true
                            font.pixelSize: 14
                            color: "#FFFFFF"
                            width: parent.width * 0.6  // occupie 60% space
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "| " + (size / (1024 * 1024)).toFixed(2) + " MB"
                            font.pixelSize: 12
                            color: "#B3B3B3"
                            width: parent.width * 0.15  // occupie 30% space
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "| " + Math.floor(duration / 60) + " min " + Math.floor(duration % 60) + " sec"
                            font.pixelSize: 12
                            color: "#B3B3B3"
                            width: parent.width * 0.15  // occupie 30% space
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: card.color = "#282828"
                        onExited: card.color = "#181818"
                        onPressed: card.scale = 0.97
                        onReleased: card.scale = 1.0
                        onClicked: {
                            ControllVideo.isstartvideo = true
                            totalTimeValue = duration
                            trackVideo.totalTimeValue = duration
                            trackVideo.totalTime = (Math.floor(duration / 60) < 10 ? "0"+ Math.floor(duration / 60) : Math.floor(duration / 60)) +
                                    ":" + ((duration % 60).toFixed(0) < 10 ? "0"+ (duration % 60).toFixed(0) : (duration % 60).toFixed(0)  )
                            console.log("Play video: " + name)
                            ControllVideo.writeCommand("VideoName="+name)
                        }
                    }
                }
            }
            clip: true
            onCountChanged: {
                totalPages = Math.ceil(videoModel.rowCount() / pageSize);
            }
        }
    }

    // Pagination bar
    Item{
        anchors.top: list_video.bottom
        width: parent.width
        height: 50

        Row {
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter
            Button {
                text: "Previous"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if (currentPage > 0) {
                        currentPage--;              // Update variable from QML
                    }
                }
            }

            Text {
                text: "Page " + (currentPage + 1) + " / " + totalPages
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                text: "Next"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if ((currentPage + 1) * pageSize < videoModel.rowCount()) {
                        currentPage++;          // update currentPage in QML
                    }
                }
            }

            TrackVideo{
                id: trackVideo
                elapsedTime: getTime()
                timePercent:  ControllVideo.time/1000/totalTimeValue
                anchors.verticalCenter: parent.verticalCenter
                function getTime(){
                    let minutes = Math.floor(ControllVideo.time/1000 / 60);
                    let seconds = parseInt(ControllVideo.time/1000) % 60;
                    return (minutes < 10 ? "0" : "") + minutes + ":" + (seconds < 10 ? "0" : "") + seconds;
                }
            }
        }
    }
    //Control video bar
    VideoControlButtons{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

}
