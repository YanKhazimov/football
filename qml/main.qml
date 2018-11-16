import QtQuick 2.7
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Window 2.0
import com.abc.abclib 1.0
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"
import "."

//https://www.futhead.com/icons/?page=1
//https://www.iconfinder.com/icons/2203522/arrow_botton_next_right_icon

QQC2.ApplicationWindow {
    id: window

    visible: true

    width: 800
    height: 600
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        setX(0);
    }

    minimumHeight: featuredStats.height + contents.height
    minimumWidth: 600//featuredStats.minWidth

    property var theme: null

    Slideshow {
        id: featuredStats
        anchors { top: window.top; horizontalCenter: parent.horizontalCenter }
        width: window.width

        title: "FEATURED STATS"
        theme: window.theme

        model: featuredStatsModel
    }

    CustomTabView {
        id: contents
        anchors { top: featuredStats.bottom }
        height: window.height - featuredStats.height
        width: window.width

        theme: window.theme
    }

    Column {
        anchors {
            bottom: parent.bottom
            bottomMargin: Sizes.featuredStats.margin
            left: parent.left
            leftMargin: Sizes.featuredStats.margin
        }
        spacing: 0

        Image {
            id: languageSwitcher
            source: "qrc:/img/lang_en.png"
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            antialiasing: true
        }

        ThemeSwitcher {
            id: themeSwitcher
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            onThemeChanged: window.theme = theme
        }
    }

    Grid
    {
        id: debugGrid
        visible: false//true
        anchors.fill: parent
        z: 1
        readonly property int size: 20
        columns: Math.ceil(width / debugGrid.size)
        rows: Math.ceil(height / debugGrid.size)
        Repeater
        {
            model: parent.visible ? parent.rows : 0
            Repeater
            {
                id: columnRepeater
                property bool evenRowIndex: index % 2 === 0
                model: debugGrid.visible ? debugGrid.columns : 0
                Rectangle
                {
                    color: "red"
                    width: debugGrid.size
                    height: debugGrid.size

                    property bool evenColumnIndex: index % 2 === 0

                    opacity:
                    {
                        if (columnRepeater.evenRowIndex)
                        {
                            return evenColumnIndex ? 0.4 : 0.2
                        }
                        return evenColumnIndex ? 0.2 : 0
                    }
                }
            }
        }
    }
}
