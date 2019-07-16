pragma Singleton
import QtQuick 2.0

// https://color.romanuke.com/tag/palitryi-dlya-dizaynera/

QtObject {
    readonly property var themes: [
        {
            "fontFamily": "Candara",
            "primaryColor": "#000020",
            "highlightColor": "dodgerblue",
            "secondaryColor": "white"
        },
        { // Цветовая палитра №1735
            "fontFamily": "Segoe Print",
            "primaryColor": "#2e2528",//"#868d99",
            "highlightColor": "#f5cc5a",
            "secondaryColor": "#868d99"//"#2e2528"
        },
        { // Цветовая палитра №3768
            "fontFamily": "Mistral",
            "primaryColor": "#005b7c",
            "highlightColor": "#01bcc6",
            "secondaryColor": "#efefee"
        }
    ]
}
