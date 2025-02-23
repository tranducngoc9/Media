import QtQuick 2.0
import QtQuick.Window 2.0
Item {
    width: 600
    height: 400
    property int currentPage: 0
    property int pageSize: 10
    property int totalPages: Math.ceil(videoModel.rowCount() / pageSize)

    // Danh sách video
    Rectangle {
        width: parent.width
        height: 350
        color: "#121212" // Màu nền tối giống Spotify

        ListView {
            id: listView
            width: parent.width
            height: parent.height
            model: videoModel.getPagedData()
            delegate: Item {
                width: listView.width
                height: 35  // Giảm chiều cao để vừa 10 phần tử
                Rectangle {
                    id: card
                    width: parent.width - 20
                    height: parent.height - 5
                    color: "#181818"  // Màu nền item
                    radius: 8
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "#1DB954" // Màu xanh đặc trưng của Spotify
                    border.width: 1.5
                    opacity: 0.95

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10  // Khoảng cách giữa các chữ

                        Text {
                            text: modelData.name
                            font.bold: true
                            font.pixelSize: 14  // Kích thước vừa phải
                            color: "#FFFFFF"
                            width: parent.width * 0.6  // Chiếm 60% không gian
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "| " + (modelData.size / (1024 * 1024)).toFixed(2) + " MB"
                            font.pixelSize: 12  // Kích thước nhỏ hơn để không tràn
                            color: "#B3B3B3"
                            width: parent.width * 0.15  // Chiếm 30% không gian
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "| " + (modelData.duration / 60).toFixed(0) + " min " + (modelData.duration % 60).toFixed(0) + " sec"
                            font.pixelSize: 12  // Kích thước nhỏ hơn để không tràn
                            color: "#B3B3B3"
                            width: parent.width * 0.15  // Chiếm 30% không gian
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
                            trackVideo.totalTime = ((modelData.duration / 60).toFixed(0)< 10 ? "0"+(modelData.duration / 60).toFixed(0) : (modelData.duration / 60).toFixed(0)) +
                                    ":" + ((modelData.duration % 60).toFixed(0) < 10 ? "0"+ (modelData.duration % 60).toFixed(0) : (modelData.duration % 60).toFixed(0)  )
                            console.log("Play video: " + modelData.name)
                            ControllVideo.writeCommand("VideoName="+modelData.name)
                        }
                    }
                }
            }
            clip: true
        }
    }


    // Thanh phân trang
    Item{
        anchors.bottom: parent.bottom
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
                        videoModel.previousPage();  // Cập nhật currentPage trong C++
                        currentPage--;              // Cập nhật biến trong QML
                        listView.model = videoModel.getPagedData();  // Lấy dữ liệu trang mới
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
                        videoModel.nextPage();  // Cập nhật currentPage trong C++
                        currentPage++;          // Cập nhật biến trong QML
                        listView.model = videoModel.getPagedData();  // Lấy dữ liệu trang mới
                    }
                }
            }
            TrackVideo{
                id: trackVideo
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

}
