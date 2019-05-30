import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/qml/visualStyles"

Rectangle {
    property int themeIndex: config.value("theme")
    property var theme: Themes.themes[themeIndex]

    radius: width
    border.color: mouseArea.containsMouse ? theme.highlightColor : theme.primaryColor
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
            config.set("theme", themeIndex)
        }
    }

    ToolTip.delay: 100
    ToolTip.timeout: 3000
    ToolTip.visible: mouseArea.containsMouse
    ToolTip.text: lang.getText("changeTheme")
}
