import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    property string source: ""
    property color color: "transparent"

    Image {
        id: img
        anchors.fill: parent
        source: root.source
    }

    ColorOverlay {
        source: img
        anchors.fill: img
        color: root.color
    }
}
