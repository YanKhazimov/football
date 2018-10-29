import QtQuick 2.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null

    width: Sizes.playerHandleWidth
    height: Sizes.playerHandleWidth + Sizes.fontPixelSize

    color: "transparent"
    border.width: 1
    border.color: "white"

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
        border.width: 1
        border.color: "white"

        Text {
            id: name
            text: hasPhoto() ? "" : player.name
            font.family: Themes.fontFamily
            anchors.centerIn: parent
        }
        Image {
            source: hasPhoto() ? player.photo : ""
            width: parent.width - 2
            height: width
            anchors.centerIn: parent
            visible: hasPhoto()
        }
    }
    Text {
        id: rating
        text: "3210"
        font.family: Themes.fontFamily
        anchors.top: pic.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
        drag.threshold: 0
    }

    Drag.hotSpot: Qt.point(width / 2, height / 2)
    Drag.dragType: Drag.Automatic
    property bool dragActive: mouseArea.drag.active

    Behavior on x { PropertyAnimation { duration: 300 } }
    Behavior on y { PropertyAnimation { duration: 300 } }
}
