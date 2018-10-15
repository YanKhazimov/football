import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "Players"
import "qrc:/qml/visualStyles"
import "."

Rectangle {
    id: root
    property QtObject theme: null
    color: root.theme.primaryFillColor

    TiledBackground {
        sourceImg: "qrc:/img/ball.png"
    }

    RowLayout {
        spacing: 0
        anchors.fill: parent

        ColumnLayout {
            id: menusArea
            Layout.preferredWidth: root.width / 4
            Layout.minimumWidth: root.width / 4
            Layout.maximumWidth: root.width / 4
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

                model: ["Players", "Calendar", "Next Game"]
                ContentTab {
                    name: modelData
                    Layout.topMargin: index === 0 ? height : 0
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: -Sizes.borderWidth//content.currentIndex === model.index ? 0 : Sizes.featuredStats.smallMargin
                    Layout.preferredWidth: defaultWidth
                    Layout.minimumWidth: defaultWidth
                    theme: root.theme
                    selected: content.currentIndex === model.index
                    onClicked: {
                        menus.select(model.index)
                        content.currentIndex = model.index
                    }
                }

                Component.onCompleted: itemAt(content.currentIndex).extend(false)
            }
        }

        ListModel {
            id: libraryModel
            ListElement {
                rank: 1
                Name: "p3"
                Rating: "500"
                Wins_Losses: "wl3"
                Reliability: "r3"
            }
            ListElement {
                rank: 2
                Name: "p1"
                Rating: "400"
                Wins_Losses: "wl1"
                Reliability: "r1"
            }
            ListElement {
                rank: 3
                Name: "p2"
                Rating: "300"
                Wins_Losses: "wl2"
                Reliability: "r2"
            }
        }

        Rectangle {
            color: root.theme.secondaryFillColor
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin
            //Layout.rightMargin: Sizes.featuredStats.smallMargin

            TabView {
                id: content
                anchors.fill: parent
                anchors.margins: Sizes.borderWidth

                tabsVisible: false
                Tab {
                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        CustomTableView {
                            id: ratingsTable
                            model: globalStatsModel
                            theme: root.theme

                            Layout.preferredWidth: parent.width / 2
                            Layout.minimumWidth: parent.width / 2
                            Layout.fillHeight: true
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
                                var player = globalStatsModel.getPlayer("a")//featuredStatsModel[1].getQueryResult()[rowIndex].player
                                playerPage.reset(player)
                            })
                        }

                        Connections {
                            target: playerPage
                            onSelectedStatChanged: ratingsTable.replaceColumn(statCategory)
                        }
                    }
                }
                Tab {
                    Rectangle { color: "blue" }
                }
                Tab {
                    Rectangle { color: "white" }
                }
            }
        }
    }
}
