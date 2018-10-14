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

    function reset(newPlayer) {
        player = newPlayer
    }

    signal selectedStatChanged(string statCategory)

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

        StatsTable {
            id: statsTable
            model: player ? playerStatsModel : 0
            theme: root.theme

            Layout.fillWidth: true
            Layout.maximumHeight: Sizes.fontPixelSize * model.length

            Connections {
                target: statsTable.selection
                onSelectionChanged: {
                    var category = statsTable.model.getStatCategory(statsTable.currentRow)
                    root.selectedStatChanged(category)
                }
            }
        }

        Chart {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin

            theme: root.theme
            points: [Qt.point(20, 3021), Qt.point(40, 1004), Qt.point(50, 8024), Qt.point(80, 1011)]
            //points: [Qt.point(20, 4021)]
        }
    }
}
