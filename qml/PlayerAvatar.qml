import QtQuick 2.6
import QtQuick.Controls 2.1
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null
    property var theme: null
    property color backColor: "white"
    property color textColor: "black"
    property var tooltipMouseArea: ma

    function hasPhoto() {
        return (player !== null) && (player.photo != "qrc:/img/playerImages/default.png")
    }

    id: pic
    implicitWidth: Sizes.playerHandleWidth
    implicitHeight: Sizes.playerHandleWidth
    radius: Sizes.playerHandleWidth/3
    color: backColor
    border.width: Sizes.borderWidth
    border.color: hasPhoto() ? "transparent" : textColor

    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true
    }

    Text {
        id: name
        text: hasPhoto() ? "" : player.shortened
        font.family: Themes.fontFamily
        font.pixelSize: Sizes.fontPixelSize
        anchors.centerIn: parent
        color: textColor
    }
    Image {
        source: hasPhoto() ? player.photo : ""
        width: parent.width
        height: width
        anchors.centerIn: parent
        visible: hasPhoto()
    }

    ToolTip.text: player.name
    ToolTip.delay: 500
    ToolTip.timeout: 3000
    ToolTip.visible: tooltipMouseArea.containsMouse
}
