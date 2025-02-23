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
        color: "green"
        opacity: 0.5

        ListView {
            id: listView
            width: parent.width
            height: parent.height
            model: videoModel.getPagedData()  // Gọi getPagedData() để chỉ lấy 10 phần tử mỗi trang
            delegate: Item {
                width: listView.width
                height: 30
                Row {
                    spacing: 10
                    Text { text: "Name: " + modelData.name }
                    Text { text: " | Size: " + modelData.size + " bytes" }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Play video " + modelData.name)
                        ControllVideo.writeCommand("VideoName="+modelData.name)
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
            Button {
                text: "Previous"
                onClicked: {
                    if (currentPage > 0) {
                        videoModel.previousPage();  // Cập nhật currentPage trong C++
                        currentPage--;              // Cập nhật biến trong QML
                        listView.model = videoModel.getPagedData();  // Lấy dữ liệu trang mới
                    }
                }
            }

            Text { text: "Page " + (currentPage + 1) + " / " + totalPages }

            Button {
                text: "Next"
                onClicked: {
                    if ((currentPage + 1) * pageSize < videoModel.rowCount()) {
                        videoModel.nextPage();  // Cập nhật currentPage trong C++
                        currentPage++;          // Cập nhật biến trong QML
                        listView.model = videoModel.getPagedData();  // Lấy dữ liệu trang mới
                    }
                }
            }
        }
    }



}
