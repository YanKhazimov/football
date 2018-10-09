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

                model: ["Players", "Calendar", "New Game"]
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
                title: "A Masterpiece"
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

        Rectangle {
            color: root.theme.secondaryFillColor
//            Layout.minimumWidth: root.width * 3/4
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

                            function updatePlayer(rowIndex) {
                                playerPage.player = fm[1].getQueryResult()[rowIndex].player
                            }

                            Connections {
                                target: ratingsTable.selection
                                onSelectionChanged: ratingsTable.selection.forEach(ratingsTable.updatePlayer)
                            }

                            Layout.preferredWidth: parent.width / 2
                            Layout.minimumWidth: parent.width / 2
                            Layout.fillHeight: true
                            model: libraryModel
                            theme: root.theme

                            TableViewColumn {
                                role: "rank"
                                title: "#"
                                width: ratingsTable.width / 10
                                resizable: false
                                movable: false
                            }
                            TableViewColumn {
                                role: "title"
                                title: "Title"
                                width: ratingsTable.width * 6/10
                                resizable: false
                            }
                            TableViewColumn {
                                role: "author"
                                title: "Rating"
                                width: ratingsTable.width * 3/10
                                resizable: false
                            }
                        }

                        PlayerPage {
                            id: playerPage
                            Layout.preferredWidth: parent.width - ratingsTable.width
                            Layout.fillHeight: true
                            theme: root.theme
                        }
                    }
                }
                Tab {
                    Rectangle { color: "Blue" }
                }
                Tab {
                    Rectangle { color: "Blue" }
                }
            }
        }
    }
}
