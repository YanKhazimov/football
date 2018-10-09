import QtQuick 2.0

Flow {
    id: flow

    property string sourceImg: ""

    clip: true
    anchors.fill: parent

    Repeater {
        id : repeater
        model: parseInt((parent.width / 32 + 1) * (parent.height / 32 + 1), 10)
        Image {
            id: img
            source: sourceImg
            width: 32
            height: width
        }
    }
}
