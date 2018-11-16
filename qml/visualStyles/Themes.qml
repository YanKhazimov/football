pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property string fontFamily: "URW Bookman L"
    readonly property color dropHighlightColor: "blue"

    readonly property var themes: [
        {
            "primaryFillColor": "#000020",
            "secondaryFillColor": "dodgerblue",
            "highlightColor": "white",
            "textColor": "white"
        },
        {
            "primaryFillColor": "red",
            "secondaryFillColor": "orange",
            "highlightColor": "yellow",
            "textColor": "pink"
        },
        {
            "primaryFillColor": "gray",
            "secondaryFillColor": "darkgray",
            "highlightColor": "white",
            "textColor": "lightgray"
        }
    ]
}
