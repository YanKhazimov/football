import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12 as QQC2
import "qrc:/qml/visualStyles"
import "../FeaturedStats"
import ".."

Rectangle {
    id: root
    color: "transparent"
    property var theme: null
    property real wingWidth: width / 20

    RowLayout {
       id: seasonsLayout
       anchors.centerIn: parent
       spacing: Sizes.featuredStats.smallMargin

       SlideshowWing {
           Layout.preferredHeight: root.height
           Layout.preferredWidth: wingWidth
           img: "qrc:/img/l.png"
           theme: root.theme
       }

       Repeater {
           model: gamesModel.getSeasons().filter(function(value){ return value != new Date().getFullYear() })
           delegate: Rectangle {
               id: seasonDelegate
               property int season: parseInt(modelData, 10)
               clip: true
               Layout.preferredHeight: root.height - 2 * Sizes.featuredStats.smallMargin
               Layout.preferredWidth: {
                   var n = 2
                   return (root.width - 2 * root.wingWidth - (n + 1) * Sizes.featuredStats.smallMargin) / n
               }
               gradient: Gradient {
                   GradientStop { position: 0.0; color: "transparent" }
                   GradientStop { position: 0.5; color: root.theme.primaryColor }
                   GradientStop { position: 1.0; color: "transparent" }
               }

               ColumnLayout {
                   spacing: Sizes.featuredStats.margin

                   Text {
                       id: seasonText
                       text: modelData
                       Layout.alignment: Qt.AlignHCenter
                       font.family: Themes.fontFamily
                       font.pointSize: 16
                       font.bold: true
                       color: root.theme.secondaryColor

                       ColoredImage {
                           source: "qrc:/img/lock.png"
                           height: seasonText.height
                           width: height
                           anchors.left: seasonText.right
                           anchors.leftMargin: Sizes.featuredStats.smallMargin
                           anchors.verticalCenter: seasonText.verticalCenter
                           color: root.theme.secondaryColor
                       }
                   }

                   Repeater {
                       id: categoryRepeater
                       model: ["Progress", "Dedication"]
                       delegate: Item {
                           id: categoryDelegate
                           Layout.preferredWidth: seasonDelegate.width
                           Layout.preferredHeight: (seasonDelegate.height - seasonText.height - Sizes.featuredStats.margin * (categoryRepeater.count + 1)) / categoryRepeater.count
                           property string category: modelData
                           ColumnLayout {
                               anchors.verticalCenter: parent.verticalCenter
                               Repeater {
                                   model: ["gold", "silver", "bronze"]
                                   delegate: RowLayout {
                                       Image {
                                           source: "qrc:/img/medal_" + categoryDelegate.category.toLowerCase() + "_" + modelData.toLowerCase() + ".png"
                                           sourceSize.width: 64
                                           sourceSize.height: 64
                                           MouseArea {
                                               anchors.fill: parent
                                               hoverEnabled: true
                                               QQC2.ToolTip.delay: 100
                                               QQC2.ToolTip.timeout: 3000
                                               QQC2.ToolTip.visible: containsMouse
                                               QQC2.ToolTip.text: categoryDelegate.category + " - " + lang.getText(modelData).toUpperCase() // TODO translate category
                                           }
                                       }
                                       Item {
                                           Layout.preferredWidth: 64
                                           Layout.preferredHeight: 64
                                           Text {
                                               anchors.centerIn: parent
                                               text: {
                                                   var awards = globalStatsModel.getAwards([categoryDelegate.category], [modelData],
                                                                                           [seasonDelegate.season], []);
                                                   return awards.length === 0 ? "-" : awards[0].score
                                               }
                                               font.family: Themes.fontFamily
                                               font.pointSize: 12
                                               font.bold: true
                                               color: root.theme.secondaryColor
                                           }
                                       }
                                       Repeater {
                                           id: playerRepeater
                                           model: {
                                               var awards = globalStatsModel.getAwards([categoryDelegate.category], [modelData],
                                                                                       [seasonDelegate.season], []);
                                               var players = []
                                               for (var i = 0; i < awards.length; i++)
                                                   players.push(awards[i].player)
                                               return players
                                           }
                                           delegate: PlayerAvatar {
                                               player: modelData
                                               theme: root.theme
                                               Layout.preferredHeight: 64
                                               Layout.preferredWidth: 64
                                               backColor: "transparent"
                                               textColor: theme.secondaryColor
                                           }
                                       }
                                   }
                               }
                           }
                       }
                   }
               }
           }
       }

       SlideshowWing {
           Layout.preferredHeight: root.height
           Layout.preferredWidth: wingWidth
           img: "qrc:/img/r.png"
           theme: root.theme
       }
    }
}
