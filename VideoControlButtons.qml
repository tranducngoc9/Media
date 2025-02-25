import QtQuick 2.0

Rectangle {
    width: 350
    height: 50
    color: "transparent"

    Row {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "<<< x0.25"
            onClicked: {
                console.log("Speed: 0.25x")
                ControllVideo.writeCommand("speed=120")
            }
        }

        Button {
            text: "<< x0.5"
            onClicked: {
                console.log("Speed: 0.5x")
                ControllVideo.writeCommand("speed=60")
            }
        }

        Button {
            id: pauseButton
            text: "|| Pause"
            onClicked: {
                if (text === "|| Pause") {
                    ControllVideo.writeCommand("status=pause")
                    text = "▶ Continue"
                    console.log("Paused")
                } else {
                    ControllVideo.writeCommand("status=resume")
                    text = "|| Pause"
                    console.log("Continued")
                }
            }
        }

        Button {
            text: "▶ x1"
            onClicked:{
                console.log("Speed: 1x")
                ControllVideo.writeCommand("speed=30")
            }
        }

        Button {
            text: ">> x2"
            onClicked:{
                console.log("Speed: 2x")
                ControllVideo.writeCommand("speed=15")
            }
        }

        Button {
            text: ">>> x3"
            onClicked:{
                console.log("Speed: 3x")
                ControllVideo.writeCommand("speed=10")
            }
        }
    }
}


