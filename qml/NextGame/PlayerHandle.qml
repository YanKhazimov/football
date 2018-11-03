import QtQuick 2.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null

    property QtObject theme: null
    width: Sizes.playerHandleWidth
    height: Sizes.playerHandleWidth + Sizes.fontPixelSize

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

        Text {
            id: name
            text: hasPhoto() ? "" : player.name
            font.family: Themes.fontFamily
            anchors.centerIn: parent
        }
        Image {
            source: hasPhoto() ? player.photo : ""
            width: parent.width
            height: width
            anchors.centerIn: parent
            visible: hasPhoto()
        }
        Rectangle {
            id: frame
            width: parent.width
            height: width
            radius: width
            color: "transparent"
            border.width: Sizes.borderWidth
            border.color: mouseArea.containsMouse ? theme.secondaryFillColor : "white"
        }
    }
    Text {
        id: rating
        text: "3210"
        font.family: Themes.fontFamily
        anchors.top: pic.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: mouseArea.containsMouse ? theme.secondaryFillColor : "white"
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
