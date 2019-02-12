import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property string source: ""
    property color color: "transparent"
    width: img.sourceSize.width
    height: img.sourceSize.height
    property int fillMode

    Image {
        id: img
        anchors.fill: parent
        source: root.source
        horizontalAlignment: Image.AlignRight
        fillMode: root.fillMode
    }

    ColorOverlay {
        source: img
        anchors.fill: img
        color: root.color
    }
}
