import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root

    anchors.fill: parent

    Rectangle {
        id: bench
        anchors {
            top: root.top
            left: root.left
            right: root.right
        }
        height: root.height / 4
        color: "grey"
    }

    Rectangle {
        id: pitch
        anchors {
            top: bench.bottom
            left: root.left
            right: root.right
            bottom: root.bottom
        }
        color: "green"
    }
}
