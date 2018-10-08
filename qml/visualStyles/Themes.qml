pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property string fontFamily: "URW Bookman L"

    property QtObject theme1: QtObject {
        readonly property color primaryFillColor: "#000020"
        readonly property color secondaryFillColor: "dodgerblue"
        readonly property color highlightColor: "white"
        readonly property color textColor: "white"
    }
    property QtObject theme2: QtObject {
        readonly property color primaryFillColor: "red"
        readonly property color secondaryFillColor: "orange"
        readonly property color highlightColor: "yellow"
        readonly property color textColor: "pink"
    }
}
