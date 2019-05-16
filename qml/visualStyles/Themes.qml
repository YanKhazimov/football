pragma Singleton
import QtQuick 2.0

// https://color.romanuke.com/tag/palitryi-dlya-dizaynera/

QtObject {
    readonly property string fontFamily: "URW Bookman L"

    readonly property var themes: [
        {
            "primaryColor": "#000020",
            "highlightColor": "dodgerblue",
            "secondaryColor": "white"
        },
        { // Цветовая палитра №1735
            "primaryColor": "#868d99",
            "highlightColor": "#f5cc5a",
            "secondaryColor": "#2e2528"
        },
        { // Цветовая палитра №3768
            "primaryColor": "#005b7c",
            "highlightColor": "#01bcc6",
            "secondaryColor": "#efefee"
        }
    ]
}
