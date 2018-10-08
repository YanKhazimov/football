import QtQuick 2.0
import QtQuick.Layouts 1.3
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    color: theme.primaryFillColor
    property QtObject theme: null
    property Player player: null

    Flow {
        id: flow
        clip: true
        anchors.fill: parent

        Repeater {
            model: 500
            Image {
                source: "qrc:/img/ball2.png"
                width: 32//flow.width / 5
                height: width
            }
        }
    }

    ColumnLayout {
        id: column
        spacing: 0
        anchors.centerIn: parent

        //RowLayout {
            Text {
                id: name
                text: player ? player.name : ""
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                color: root.theme.textColor
            }
            Image {
                id: photo
                source: player ? player.photo : ""
            }
        //}

        Repeater {
            id: textStats
            model: statsExample
            delegate: Text {
                text: player ? modelData : ""
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                color: root.theme.textColor
            }
        }
    }
}
