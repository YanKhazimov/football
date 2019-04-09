import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    property int themeIndex: 0
    property var theme: Themes.themes[themeIndex]

    radius: width
    border.color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.primaryFillColor
    border.width: Sizes.borderWidth

    Image {
        source: "qrc:/img/colorball.png"
        anchors.fill: parent
        antialiasing: true
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            //lang.set(themeIndex % 2 === 0 ? "en" : "ru")
            themeIndex = (themeIndex + 1) % Themes.themes.length
            theme = Themes.themes[themeIndex]
        }
    }
}
