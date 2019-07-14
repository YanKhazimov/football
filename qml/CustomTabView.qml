import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "Players"
import "NextGame"
import "Awards"
import "qrc:/qml/visualStyles"
import "."
import com.abc.abclib 1.0

Rectangle {
    id: root
    property var theme: null
    color: root.theme.primaryColor
    property string activeTab: lang.playersTab

    ColoredImage {
        id: bg
        anchors.fill: parent
        source: "qrc:/img/corner.jpg"
        color: {
            var hex = theme.primaryColor
            var r = "0x" + hex[1] + hex[2]
            var g = "0x" + hex[3] + hex[4]
            var b = "0x" + hex[5] + hex[6]
            return Qt.rgba(r, g, b, 0.2)
        }
    }

    RowLayout {
        spacing: 0
        anchors.fill: parent

        ColumnLayout {
            id: menusArea
            Layout.preferredWidth: root.width / 6
            Layout.minimumWidth: root.width / 6
            Layout.maximumWidth: root.width / 6
            Layout.alignment: Qt.AlignTop

            Repeater {
                id: menus

                function select(idx) {
                    for (var i = 0; i < model.length; ++i)
                    {
                        if (i === idx)
                            itemAt(i).extend()
                        else
                            itemAt(i).reset()
                    }
                }

                model: [lang.playersTab, lang.nextGameTab, lang.calendarTab, lang.rewardsTab]
                ContentTab {
                    name: modelData
                    Layout.topMargin: index === 0 ? height : 0
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: -Sizes.borderWidth
                    Layout.preferredWidth: defaultWidth
                    Layout.minimumWidth: defaultWidth
                    theme: root.theme
                    selected: root.activeTab == modelData
                    onClicked: {
                        menus.select(model.index)
                        root.activeTab = modelData
                    }
                    enabled: model.index !== 2
                    visible: model.index < 4
                }

                Component.onCompleted: itemAt(0).extend(false)
            }
        }

        Rectangle {
            visible: root.activeTab == lang.playersTab
            border.width: Sizes.borderWidth
            border.color: theme.highlightColor
            color: "transparent"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            RowLayout {
                spacing: 0
                anchors.fill: parent
                anchors.margins: Sizes.borderWidth

                StatsTable {
                    id: ratingsTable
                    property GlobalStatPresenter presenter: statModel
                    model: presenter
                    theme: root.theme

                    Layout.preferredWidth: parent.width / 2
                    Layout.minimumWidth: parent.width / 2
                    Layout.fillHeight: true
                }

                Connections {
                    target: ratingsTable.presenter
                    onSelectedRowChanged: {
                        ratingsTable.selection.clear()
                        if (selectedRow !== -1)
                            ratingsTable.selection.select(selectedRow)
                    }
                }

                PlayerPage {
                    id: playerPage
                    Layout.preferredWidth: parent.width - ratingsTable.width
                    Layout.fillHeight: true
                    theme: root.theme
                }

                Connections {
                    target: ratingsTable.selection
                    onSelectionChanged: ratingsTable.selection.forEach( function(rowIndex) {
                        ratingsTable.presenter.selectRow(rowIndex)
                    })
                }

                Connections {
                    target: playerPage
                    onSelectedStatChanged: ratingsTable.replaceColumn(statCategory)
                }
                Connections {
                    target: playerPage.table.selection
                    onSelectionChanged: {
                        if (playerPage.table.selection.count !== 1) {
                            print("playerPage.table.selection.count=", playerPage.table.selection.count)
                            return
                        }
                        playerPage.table.selection.forEach( function(rowIndex) {
                            var statRole = playerPage.table.presenter.getStatRole(rowIndex)
                            ratingsTable.presenter.sortBy(statRole)
                        })
                    }
                }
            }
        }

        Item {
            visible: root.activeTab == lang.nextGameTab
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            GameComposer {
                border.width: Sizes.borderWidth
                border.color: theme.highlightColor
                id: composer
                anchors.fill: parent
                allPlayersModel: sortingStatModel
                theme: root.theme

                Connections {
                    target: root
                    onActiveTabChanged: {
                        if (root.activeTab == lang.nextGameTab)
                            composer.showControlPanel()
                    }
                }
            }
        }

        Rectangle {
            visible: root.activeTab == lang.rewardsTab
            border.width: Sizes.borderWidth
            border.color: theme.highlightColor
            color: "transparent"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            RowLayout {
                id: awardsScreenLayout
                spacing: 0
                anchors.fill: parent
                anchors.margins: Sizes.borderWidth
                property int completedSeasons: gamesModel.getSeasons().length

                CompletedSeasonAwards {
                    id: completedAwards
                    theme: root.theme

                    Layout.preferredWidth: parent.width / awardsScreenLayout.completedSeasons * (awardsScreenLayout.completedSeasons - 1)
                    Layout.minimumWidth: parent.width / awardsScreenLayout.completedSeasons * (awardsScreenLayout.completedSeasons - 1)
                    Layout.fillHeight: true
                }

                Rectangle {
                    Layout.preferredWidth: Sizes.borderWidth
                    Layout.fillHeight: true
                    color: root.theme.highlightColor
                }

                CurrentSeasonAwardRace {
                    id: currentAwards
                    theme: root.theme

                    Layout.preferredWidth: parent.width - completedAwards.width
                    Layout.fillHeight: true
                }
            }
        }
    }
}
