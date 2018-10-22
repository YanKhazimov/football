import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    property string photoSource: ""
    property string topText: ""

    width: Sizes.playerHandleSize
    height: Sizes.playerHandleSize
    Drag.active: mouseArea.drag.active
    Drag.hotSpot: Qt.point(width/2, height/2)

    color: "transparent"
    border.width: 1
    border.color: "white"

    Text {
        id: name
        text: topText
        font.family: Themes.fontFamily
        anchors.centerIn: parent
        visible: photoSource === ""
    }
    Image {
        id: pic
        source: photoSource
        width: parent.width - 2
        height: width
        anchors.centerIn: parent
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
    }
}
