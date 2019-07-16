import QtQuick 2.6
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    property Player player: null
    property var theme: null
    property color backColor: "white"
    property color textColor: "black"
    property var tooltipMouseArea: ma

    function hasPhoto() {
        return (player !== null) && (player.photo != "qrc:/img/defaultphoto.png")
    }

    id: pic
    implicitWidth: Sizes.playerHandleWidth
    implicitHeight: Sizes.playerHandleWidth
    radius: width / 3
    color: backColor
    border.width: Sizes.borderWidth
    border.color: textColor
    clip: true

    Text {
        id: name
        text: player.shortened
        font.family: theme.fontFamily
        font.pixelSize: Sizes.fontPixelSize
        anchors.centerIn: parent
        color: textColor
        visible: !hasPhoto()
    }
    Image {
        id: img
        source: hasPhoto() ? player.photo : ""
        width: parent.width
        height: width
        anchors.centerIn: parent
        visible: hasPhoto()

        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                anchors.centerIn: parent
                width: img.width
                height: width
                radius: width / 2
            }
        }
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true

        ToolTip.text: player.name
        ToolTip.delay: 500
        ToolTip.timeout: 3000
        ToolTip.visible: tooltipMouseArea.containsMouse
    }
}
