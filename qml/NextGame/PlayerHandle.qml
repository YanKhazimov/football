import QtQuick 2.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"
import ".."

Rectangle {
    id: root
    property Player player: null

    property var theme: null
    width: pic.width
    height: pic.height + Sizes.playerHandleRatingHeight

    color: "transparent"

    PlayerAvatar {
        id: pic
        player: root.player
        theme: root.theme
        textColor: mouseArea.containsMouse ? theme.secondaryFillColor : theme.primaryFillColor
        border.color: mouseArea.containsMouse ? theme.secondaryFillColor : "transparent"
        tooltipMouseArea: mouseArea
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
            text: sortingStatModel.getPlayerRating(player.name)
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
