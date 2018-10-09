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
        anchors.centerIn: parent

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

        TableView {

            ListModel {
                id: libraryModel
                ListElement {
                    rank: 1
                    title: "Current Rating"
                    author: "500"
                }
                ListElement {
                    rank: 2
                    title: "Brilliance"
                    author: "400"
                }
                ListElement {
                    rank: 3
                    title: "Outstanding"
                    author: "300"
                }
            }
            model: libraryModel

            TableViewColumn {
                delegate: Text {
                    text: styleData.value
                }
                role: "title"
                //title: "Title"
                //width: ratingsTable.width * 6/10
                //resizable: false
            }

        }
    }
}
