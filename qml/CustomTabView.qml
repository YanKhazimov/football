import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "Players"
import "NewGame"
import "qrc:/qml/visualStyles"
import "."

Rectangle {
    id: root
    property QtObject theme: null
    color: root.theme.primaryFillColor

    TiledBackground {
        sourceImg: "qrc:/img/ball.png"
    }

    Column {
        anchors {
            bottom: parent.bottom
            bottomMargin: Sizes.featuredStats.margin
            left: parent.left
            leftMargin: Sizes.featuredStats.margin
        }
        spacing: 0

        Image {
            id: languageSwitcher
            source: "qrc:/img/lang_en.png"
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            antialiasing: true
        }

        Image {
            id: themeBall

            source: "qrc:/img/colorball.png"
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            antialiasing: true
        }
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

        Rectangle {
            color: root.theme.secondaryFillColor
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            TabView {
                id: content
                anchors.fill: parent
                anchors.margins: Sizes.borderWidth

                tabsVisible: false
                Tab {
                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        StatsTable {
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
                                var player = globalStatsModel.getPlayer("p1")//featuredStatsModel[1].getQueryResult()[rowIndex].player
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
                    GameComposer {

                    }
                }
            }
        }
    }
}
