import QtQuick 2.0
import "qrc:/qml/visualStyles"
import ".."

Item {
    id: root
    property var theme: null
    property var statGroupsModel: null
    readonly property real groupSpacerMultiplier: 1.0
    readonly property real imgSpacerMultiplier: 0.1
    readonly property real maxFittingImageWidth: width / elemsAsImages()

    function elemsAsImages() {
        // returns the amount of images in statGroupsModel counting spacers as image parts

        if (statGroupsModel === null)
            return 0

        var groupSpacers = statGroupsModel.length - 1
        var images = 0
        var imgSpacers = 0
        for (var i = 0; i < statGroupsModel.length; ++i)
        {
            images += statGroupsModel[i].group.length
            imgSpacers += statGroupsModel[i].group.length - 1
        }

        return images + imgSpacers*imgSpacerMultiplier + groupSpacers*groupSpacerMultiplier
    }

    Row {
        anchors.centerIn: root
        spacing: root.groupSpacerMultiplier * root.maxFittingImageWidth
        Repeater {
            model: statGroupsModel
            delegate: Column {
                Row {
                    id: statGroup
                    spacing: root.imgSpacerMultiplier * root.maxFittingImageWidth
                    Repeater {
                        model: modelData.group
                        delegate: Column {
                            PlayerAvatar {
                                id: playerImg
                                player: modelData.player
                                theme: root.theme
                                width: Math.min(Sizes.featuredStats.iconWidth,
                                                root.maxFittingImageWidth)
                                height: width
                                backColor: theme.primaryColor
                                textColor: theme.secondaryColor
                            }

                            Text {
                                text: modelData.statValue
                                anchors.horizontalCenter: playerImg.horizontalCenter
                                color: theme.secondaryColor
                                font.family: theme.fontFamily
                            }
                        }
                    }
                }
                Text {
                    text: modelData.groupStatValue
                    anchors.horizontalCenter: statGroup.horizontalCenter
                    color: theme.secondaryColor
                    font.family: theme.fontFamily
                }
            }
        }
    }
}
