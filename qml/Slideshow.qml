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
    height: 2 + 10 + playPause.height + getContentHeight() + 10 + 2
    color: theme.primaryColor
    border.width: Sizes.borderWidth
    border.color: theme.highlightColor

    function getContentHeight() {
        return Sizes.featuredStats.iconHeight + Sizes.fontPixelSize + Sizes.fontPixelSize
    }

    Item {
        anchors {
            top: parent.top
            left: parent.left; leftMargin: Sizes.borderWidth
            right: parent.right; rightMargin: Sizes.borderWidth
            bottom: parent.bottom; bottomMargin: Sizes.borderWidth
        }

        SlideshowWing {
            id: leftWing
            anchors {
                left: parent.left
                top: playPause.bottom
                topMargin: Sizes.smallMargin
                bottom: parent.bottom
                bottomMargin: Sizes.smallMargin
            }
            img: "qrc:/img/l.png"
            theme: root.theme
        }

        SlideshowWing {
            id: rightWing
            anchors {
                right: parent.right
                top: playPause.bottom
                topMargin: Sizes.smallMargin
                bottom: parent.bottom
                bottomMargin: Sizes.smallMargin
            }
            img: "qrc:/img/r.png"
            theme: root.theme
        }

        ColoredImage {
            id: playPause
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            anchors {
                top: parent.top
                topMargin: Sizes.smallMargin
                horizontalCenter: parent.horizontalCenter
            }
            source: slideFlickTimer.running ? "qrc:/img/pause.png" : "qrc:/img/play.png"
            color: mouseArea.containsMouse ? theme.highlightColor : theme.secondaryColor

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
                interval: 10000
                repeat: true
                running: true

                onTriggered: listView.incrementCurrentIndex()
            }
        }
    }
}
