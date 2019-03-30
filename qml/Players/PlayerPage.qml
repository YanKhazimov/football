import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"
import ".."

Rectangle {
    id: root
    color: theme.primaryFillColor
    property var theme: null
    property Player player: statsTable.presenter.selectedPlayer
    property var table: statsTable
    signal selectedStatChanged(string statCategory)

    ColumnLayout {
        id: column
        spacing: player ? 5 : Sizes.featuredStats.margin
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        Text {
            id: name
            text: player ? player.name : "No player selected"
            font.family: Themes.fontFamily
            font.pixelSize: Sizes.fontPixelSize
            color: root.theme.textColor
            Layout.alignment: Qt.AlignHCenter
        }

        Image {
            source: player ? player.photo : "qrc:/img/playerImages/default.png"
            Layout.preferredHeight: 128
            Layout.preferredWidth: 128
            Layout.alignment: Qt.AlignHCenter
        }

        PlayerStatsTable {
            id: statsTable
            visible: player
            property PlayerStatsModel presenter: playerStatsModel
            model: presenter

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
            id: chart
            visible: player
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin

            theme: root.theme
            points: []

            Connections {
                target: statsTable.presenter
                onPlayerChanged: chart.updatePoints(statsTable.presenter.ratingHistory)
            }
        }
    }
}
