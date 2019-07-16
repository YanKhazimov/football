import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/qml/visualStyles"

Rectangle {
    property int curIndex: 0
    property var model
    color: "transparent"

    Component.onCompleted: {
        model = [lang.all].concat(gamesModel.getSeasons())
        globalStatsModel.setSeasonFilter(model[curIndex])
    }

    Image {
        source: "qrc:/img/season.png"
        anchors.fill: parent
        antialiasing: true
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            curIndex = (curIndex + 1) % model.length
            globalStatsModel.setSeasonFilter(model[curIndex])
        }
    }
    Text {
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
            verticalCenterOffset: height/2
        }
        text: model[curIndex]
        font.family: theme.fontFamily
        font.bold: true
        font.pixelSize: 10
    }

    ToolTip.delay: 100
    ToolTip.timeout: 3000
    ToolTip.visible: mouseArea.containsMouse
    ToolTip.text: lang.getText("changeSeason")
}
