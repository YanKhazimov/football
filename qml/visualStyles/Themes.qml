pragma Singleton
import QtQuick 2.0

// https://color.romanuke.com/tag/palitryi-dlya-dizaynera/

Item {
    FontLoader {
        id: classic
        source: "qrc:/fonts/Alice-Regular.ttf"
    }
    FontLoader {
        id: funky
        source: "qrc:/fonts/stencil.ttf"
    }
    FontLoader {
        id: lovely
        source: "qrc:/fonts/Nautilus.otf"
    }

    readonly property var themes: [
        {
            "fontFamily": classic.name,
            "primaryColor": "#000020",
            "highlightColor": "#1e90ff",
            "secondaryColor": "#ffffff"
        },
        { // Цветовая палитра №1735
            "fontFamily": funky.name,
            "primaryColor": "#868d99",//"#2e2528",
            "highlightColor": "#f5cc5a",
            "secondaryColor": "#2e2528"//"#868d99"
        },
        { // Цветовая палитра №3768
            "fontFamily": lovely.name,
            "primaryColor": "#01bcc6",//"#005b7c",
            "highlightColor": "#efefee",//"#01bcc6",
            "secondaryColor": "#6b2f7a"//"#ad59c9"
        }
    ]
}
