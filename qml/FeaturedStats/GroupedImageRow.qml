import QtQuick 2.0
import "qrc:/qml/visualStyles"

Item {
    id: flowRect
    property QtObject theme: null
    readonly property real groupSpacerMultiplier: 1.0
    readonly property real imgSpacerMultiplier: 0.1
    readonly property real maxFittingImageWidth: width / elemsAsImages()

    function elemsAsImages() {
        // returns the amount of images in statGroupsModel counting spacers as image parts

        if (contentRoot.statGroupsModel === null)
            return 0

        var groupSpacers = contentRoot.statGroupsModel.length - 1
        var images = 0
        var imgSpacers = 0
        for (var i = 0; i < contentRoot.statGroupsModel.length; ++i)
        {
            images += contentRoot.statGroupsModel[i].group.length
            imgSpacers += contentRoot.statGroupsModel[i].group.length - 1
        }

        return images + imgSpacers*imgSpacerMultiplier + groupSpacers*groupSpacerMultiplier
    }

    Row {
        anchors.centerIn: flowRect
        spacing: flowRect.groupSpacerMultiplier * flowRect.maxFittingImageWidth
        Repeater {
            model: contentRoot.statGroupsModel
            delegate: Column {
                Row {
                    id: statGroup
                    spacing: flowRect.imgSpacerMultiplier * flowRect.maxFittingImageWidth
                    Repeater {
                        model: modelData.group
                        delegate: Column {
                            Image {
                                id: playerImg
                                width: Math.min(Sizes.featuredStats.iconWidth,
                                                flowRect.maxFittingImageWidth)
                                height: width
                                source: modelData.player.photo
                            }
                            Text {
                                text: modelData.statValue
                                anchors.horizontalCenter: playerImg.horizontalCenter
                                color: theme.textColor
                                font.family: Themes.fontFamily
                            }
                        }
                    }
                }
                Text {
                    text: modelData.groupStatValue
                    anchors.horizontalCenter: statGroup.horizontalCenter
                    color: theme.textColor
                    font.family: Themes.fontFamily
                }
            }
        }
    }
}
