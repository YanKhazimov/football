import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.12 as QQC2
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"
import ".."

Rectangle {
    id: root
    color: "transparent"
    property var theme: null
    property Player player: statsTable.presenter.selectedPlayer
    property var table: statsTable
    signal selectedStatChanged(string statCategory)

    ColumnLayout {
        id: column
        spacing: 0
        anchors.fill: parent

        Rectangle {
            id: playerCard
            color: theme.primaryColor
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.minimumHeight: parent.height / 3
            Layout.fillHeight: true
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin
            Layout.topMargin: Sizes.featuredStats.smallMargin

            ColumnLayout {
                id: playerCardLayout
                anchors.centerIn: parent

                Text {
                    id: name
                    text: player ? player.name : "No player selected"
                    font.family: theme.fontFamily
                    font.pixelSize: Sizes.fontPixelSize
                    color: root.theme.secondaryColor
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter

                    ColoredImage {
                        source: player ? player.photo : "qrc:/img/defaultphoto.png"
                        width: 128
                        height: 128
                        Layout.alignment: Qt.AlignVCenter
                        color: ((player !== null) && (player.photo != "qrc:/img/defaultphoto.png")) ? "transparent" : root.theme.secondaryColor
                    }

                    Column {
                        Layout.alignment: Qt.AlignVCenter
                        Repeater {
                            model: globalStatsModel.getAwards([], [], [], [player.name])
                            delegate: Image {
                                source: modelData.getImageSource()
                                height: 32
                                width: 32
                                MouseArea {
                                    id: imgMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                QQC2.ToolTip.delay: 100
                                QQC2.ToolTip.timeout: 3000
                                QQC2.ToolTip.visible: imgMouseArea.containsMouse
                                QQC2.ToolTip.text: "%1 %2 - %3 (%4)".
                                    arg(modelData.season).
                                    arg(lang.getText(modelData.category.toLowerCase())).
                                    arg(lang.getText(modelData.rank.toLowerCase()).toUpperCase()).
                                    arg(modelData.score)
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            visible: player
            color: theme.primaryColor
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin
            Layout.fillWidth: true
            Layout.minimumHeight: Sizes.fontPixelSize * statsTable.presenter.length
            Layout.fillHeight: true

            PlayerStatsTable {
                id: statsTable
                property PersonalStatsModel presenter: personalStatsModel
                model: presenter
                anchors.fill: parent
                theme: root.theme

                Connections {
                    target: statsTable.selection
                    onSelectionChanged: {
                        var category = statsTable.model.getStatCategory(statsTable.currentRow)
                        root.selectedStatChanged(category)
                    }
                }
            }
        }

        Chart {
            id: chart
            visible: player
            Layout.fillWidth: true
            Layout.preferredHeight: width / 3
            Layout.fillHeight: true
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            gradient: Gradient {
                GradientStop { position: 0.0; color: root.theme.primaryColor }
                GradientStop { position: 1.0; color: "transparent" }
            }

            theme: root.theme
            points: []

            Connections {
                target: statsTable.presenter
                onPlayerChanged: chart.updatePoints(statsTable.presenter.ratingHistory)
            }

            onThemeChanged: updatePoints(statsTable.presenter.ratingHistory)
        }
    }
}
