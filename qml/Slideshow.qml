import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"
import "FeaturedStats"

Rectangle {
    id: root
    property string title: ""
    property var theme: null
    property var model: null
    width: parent.width
    height: 2 + headerText.height + separator.height + 10 + playPause.height + getContentHeight() + 10 + 2
    color: theme.primaryFillColor
    border.width: Sizes.borderWidth
    border.color: theme.secondaryFillColor

    function getContentHeight() {
        return Sizes.featuredStats.iconHeight + Sizes.fontPixelSize + Sizes.fontPixelSize
    }

    Text {
        id: headerText
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        text: root.title
        color: root.theme.textColor
        font.family: Themes.fontFamily
        font.pixelSize: Sizes.fontPixelSize
        font.bold: true
    }

    Rectangle {
        id: separator
        anchors {
            top: headerText.bottom;
            left: parent.left
            leftMargin: Sizes.borderWidth + Sizes.featuredStats.margin
            right: parent.right
            rightMargin: Sizes.borderWidth + Sizes.featuredStats.margin
        }
        color: theme.secondaryFillColor
        height: Sizes.borderWidth
    }

    Item {
        anchors {
            top: separator.bottom
            left: parent.left; leftMargin: Sizes.borderWidth
            right: parent.right; rightMargin: Sizes.borderWidth
            bottom: parent.bottom; bottomMargin: Sizes.borderWidth
        }

        SlideshowWing {
            id: leftWing
            anchors {
                left: parent.left
                top: playPause.bottom
                topMargin: Sizes.featuredStats.smallMargin
                bottom: parent.bottom
                bottomMargin: Sizes.featuredStats.smallMargin
            }
            img: "qrc:/img/l.svg"
            theme: root.theme
        }

        SlideshowWing {
            id: rightWing
            anchors {
                right: parent.right
                top: playPause.bottom
                topMargin: Sizes.featuredStats.smallMargin
                bottom: parent.bottom
                bottomMargin: Sizes.featuredStats.smallMargin
            }
            img: "qrc:/img/r.svg"
            theme: root.theme
        }

        ColoredImage {
            id: playPause
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            anchors {
                top: parent.top
                topMargin: Sizes.featuredStats.smallMargin
                horizontalCenter: parent.horizontalCenter
            }
            source: slideFlickTimer.running ? "qrc:/img/pause.png" : "qrc:/img/play.png"
            color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.highlightColor

            MouseArea {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    slideFlickTimer.running = !slideFlickTimer.running
                    slideFlickTimer.triggeredOnStart = true
                }
            }
        }

        ListView {
            id: listView

            anchors {
                left: leftWing.right
                right: rightWing.left
                top: playPause.bottom
                bottom: parent.bottom
            }

            onCurrentIndexChanged: {
                leftWing.update(currentIndex > 0)
                rightWing.update(currentIndex < model.rowCount() - 1)
            }

            clip: true
            orientation: ListView.Horizontal
            keyNavigationWraps: true
            snapMode: ListView.SnapOneItem
            interactive: false
            highlightMoveDuration: 300

            model: root.model
            delegate: SlideContent {
                title: ListView.isCurrentItem ? model.name : ""
                descr: ListView.isCurrentItem ? model.description : ""
                theme: root.theme
                statGroupsModel: ListView.isCurrentItem ? model.queryResult : null
                width: listView.width
                height: listView.height
            }

            Component.onCompleted: {
                leftWing.clicked.connect(function(){
                    decrementCurrentIndex()
                    slideFlickTimer.stop()
                })
                rightWing.clicked.connect(function(){
                    incrementCurrentIndex()
                    slideFlickTimer.stop()
                })
            }

            Timer {
                id: slideFlickTimer
                interval: 5000
                repeat: true
                running: false

                onTriggered: listView.incrementCurrentIndex()
            }
        }
    }
}
