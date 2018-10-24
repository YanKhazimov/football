import QtQuick 2.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null

    width: Sizes.playerHandleSize
    height: Sizes.playerHandleSize
    radius: Sizes.playerHandleSize

    color: "transparent"
    border.width: 1
    border.color: "white"

    function hasPhoto() {
        return (player !== null) && (player.photo != "qrc:/img/playerImages/default.png")
    }

    Text {
        id: name
        text: hasPhoto() ? "" : player.name
        font.family: Themes.fontFamily
        anchors.centerIn: parent
    }
    Image {
        id: pic
        source: hasPhoto() ? player.photo : ""
        width: parent.width - 2
        height: width
        anchors.centerIn: parent
        visible: hasPhoto() ? player.name : ""
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
    }

    Drag.hotSpot: Qt.point(width / 2, height / 2)
    Drag.dragType: Drag.Automatic
    property bool dragActive: mouseArea.drag.active
}
