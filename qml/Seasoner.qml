import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    property int curIndex: 0
    property var model
    color: "transparent"
    border.color: mouseArea.containsMouse ? theme.secondaryFillColor : "transparent"
    border.width: Sizes.borderWidth

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
        font.family: Themes.fontFamily
        font.bold: true
        font.pixelSize: 10
    }
}
