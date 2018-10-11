pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property size elementButtonSize: Qt.size(32, 32)

    readonly property int borderWidth: 2
    readonly property int fontPixelSize: 20

    property QtObject featuredStats: QtObject {
        readonly property real wingShare: 0.1
        readonly property int margin: 20
        readonly property int smallMargin: 10
        readonly property int iconWidth: 64
        readonly property int iconHeight: 64
    }
}
