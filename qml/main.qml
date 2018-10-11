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

    Slideshow {
        id: featuredStats
        anchors { top: window.top; horizontalCenter: parent.horizontalCenter }
        width: window.width

        title: "FEATURED STATS"
        theme: Themes.theme1
        model: featuredStatsModel
    }

    CustomTabView {
        id: contents
        anchors { top: featuredStats.bottom }
        height: window.height - featuredStats.height
        width: window.width

        theme: Themes.theme1
    }
}
