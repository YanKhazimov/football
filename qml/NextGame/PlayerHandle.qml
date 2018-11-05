import QtQuick 2.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null

    property QtObject theme: null
    width: Sizes.playerHandleWidth
    height: Sizes.playerHandleWidth + Sizes.playerHandleRatingHeight

    color: "transparent"

    function hasPhoto() {
        return (player !== null) && (player.photo != "qrc:/img/playerImages/default.png")
    }

    Rectangle {
        id: pic
        width: Sizes.playerHandleWidth
        height: Sizes.playerHandleWidth
        radius: Sizes.playerHandleWidth
        anchors.top: parent.top
        color: "transparent"

        Rectangle {
            width: parent.width
            height: width
            radius: width
            color: "white"
            border.width: Sizes.borderWidth
            border.color: mouseArea.containsMouse ? theme.secondaryFillColor : "white"
        }
        Text {
            id: name
            text: hasPhoto() ? "" : player.name
            font.family: Themes.fontFamily
            font.pixelSize: Sizes.fontPixelSize
            anchors.centerIn: parent
            color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.primaryFillColor
        }
        Image {
            source: hasPhoto() ? player.photo : ""
            width: parent.width
            height: width
            anchors.centerIn: parent
            visible: hasPhoto()
        }
    }
    Rectangle {
        id: rating
        anchors.top: pic.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: Sizes.playerHandleRatingHeight
        radius: height / 4
        color: "white"
        Text {
            text: "3210"
            font.family: Themes.fontFamily
            font.bold: true
            anchors.centerIn: parent
            color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.primaryFillColor
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        drag.target: parent
        drag.threshold: 0
    }

    Drag.hotSpot: Qt.point(width / 2, height / 2)
    Drag.dragType: Drag.Automatic
    property bool dragActive: mouseArea.drag.active

    Behavior on x { PropertyAnimation { duration: 300 } }
    Behavior on y { PropertyAnimation { duration: 300 } }
}
