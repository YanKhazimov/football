import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    property int curIndex: 0
    property var model: ["All"]
    color: "transparent"

    Image {
        source: "qrc:/img/season.png"
        anchors.fill: parent
        antialiasing: true
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            curIndex = (curIndex + 1) % model.length
            gamesFilterModel.setSeasonFilter(model[curIndex])
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
