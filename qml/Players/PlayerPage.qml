import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"
import ".."

Rectangle {
    id: root
    color: theme.primaryFillColor
    property QtObject theme: null
    property Player player: null

    TiledBackground {
        sourceImg: "qrc:/img/ball2.png"
    }

    ColumnLayout {
        id: column
        spacing: 5
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        Text {
            id: name
            text: player ? player.name : ""
            font.family: Themes.fontFamily
            font.pixelSize: Sizes.fontPixelSize
            color: root.theme.textColor
            Layout.alignment: Qt.AlignHCenter
        }
        Image {
            id: photo
            source: player ? player.photo : ""
            Layout.alignment: Qt.AlignHCenter
        }

//        Repeater {
//            id: textStats
//            model: statsExample
//            delegate: Text {
//                text: player ? modelData : ""
//                font.family: Themes.fontFamily
//                font.pixelSize: Sizes.fontPixelSize
//                color: root.theme.textColor
//                Layout.alignment: Qt.AlignHCenter
//            }
//        }

        StatsTable {
            id: statsTable
            model: player ? statsExample : 0

            Layout.fillWidth: true
            Layout.maximumHeight: Sizes.fontPixelSize * model.length

            TableViewColumn {
                delegate: Text {
                    text: model.StatCategory + ":"
                    horizontalAlignment: Text.AlignHCenter
                }
                role: "StatCategory"
                width: statsTable.width * 2/5
            }

            TableViewColumn {
                delegate: Text {
                    text: model.StatValue
                    horizontalAlignment: Text.AlignHCenter
                }
                role: "StatValue"
                width: statsTable.width * 2/5
            }

            TableViewColumn {
                delegate: Text {
                    text: "sort"
                    horizontalAlignment: Text.AlignHCenter
                }
                width: statsTable.width / 5
            }
        }
    }
}
