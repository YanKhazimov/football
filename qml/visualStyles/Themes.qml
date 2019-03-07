pragma Singleton
import QtQuick 2.0

// https://color.romanuke.com/tag/palitryi-dlya-dizaynera/

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
        { // Цветовая палитра №2814
            "primaryFillColor": "#ede4d4",
            "secondaryFillColor": "#f98e8e",
            "highlightColor": "#e64b55",
            "textColor": "#604f45"
        },
        { // Цветовая палитра №3768
            "primaryFillColor": "#005b7c",
            "secondaryFillColor": "#01bcc6",
            "highlightColor": "#d5d1ca",
            "textColor": "#efefee"
        },
        { // Цветовая палитра №1735
            "primaryFillColor": "#868d99",
            "secondaryFillColor": "#f5cc5a",
            "highlightColor": "#fefefe",
            "textColor": "#2e2528"
        }
    ]
}
