import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "Players"
import "NextGame"
import "qrc:/qml/visualStyles"
import "."
import com.abc.abclib 1.0

Rectangle {
    id: root
    property var theme: null
    color: root.theme.primaryFillColor

//    TiledBackground {
//        sourceImg: "qrc:/img/ball.png"
//        baseColor: theme.primaryFillColor
//    }

    Image {
        id: bg
        source: "qrc:/img/corner.jpg"
        anchors.fill: parent
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

                model: ["Players", "Next Game", "Calendar"]
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
                    enabled: model.index < 2
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
                                console.debug(selectedRow)
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
                Tab {
                    GameComposer {
                        id: composer
                        allPlayersModel: sortingStatModel
                        theme: root.theme

                        Connections {
                            target: content
                            onCurrentIndexChanged: {
                                if (content.currentIndex == 1)
                                    composer.showControlPanel()
                            }
                        }
                    }
                }
                Tab {
                    Rectangle { color: "blue" }
                }
            }
        }
    }
}
