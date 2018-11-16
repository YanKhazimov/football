import QtQuick 2.0

Flow {
    id: flow

    property string sourceImg: ""
    property color baseColor: null

    clip: true
    anchors.fill: parent

    Repeater {
        id : repeater
        model: parseInt((parent.width / 32 + 1) * (parent.height / 32 + 1), 10)
        ColoredImage {
            source: sourceImg
            color: {
                var themeColor = Qt.darker(baseColor, 1)
                var lighnessThreshold = 0.3
                if (themeColor.hslLightness < lighnessThreshold)
                    return Qt.lighter(baseColor, 1.5)
                else
                    return Qt.darker(baseColor, 1.5)
            }
            width: 32
            height: width
        }
    }
}
