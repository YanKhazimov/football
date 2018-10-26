import QtQuick 2.0
import "."
import "qrc:/qml/visualStyles"

Rectangle {
    id: pitchScheme
    anchors.fill: parent

    property point pitchCenterCoords: Qt.point(pitchCenter.x + pitchCenter.width/2,
                                               bench.height + pitchCenter.y + pitchCenter.height/2)
    property rect centralZone: Qt.rect(pitchCenter.x, bench.height + pitchCenter.y,
                                       pitchCenter.width, pitchCenter.height)
    property rect leftZone: Qt.rect(pitchLeftHalf.x, bench.height + pitchLeftHalf.y,
                                    pitchLeftHalf.width, pitchLeftHalf.height)
    property rect rightZone: Qt.rect(pitchRightHalf.x, bench.height + pitchRightHalf.y,
                                     pitchRightHalf.width, pitchRightHalf.height)
    property real benchHeight: height / 4
    property int benchSpacing: Sizes.featuredStats.smallMargin / 2

    function benchPlayerZone(index) {

        var x = index % benchGrid.columns * (benchSpacing + Sizes.playerHandleSize)
        var y = Math.floor(index / benchGrid.columns) * (benchSpacing + Sizes.playerHandleSize)

        return Qt.point(benchGrid.x + x,
                        benchGrid.y + y)
    }

    property int benchLength: 0

    signal dragEnter(int zone)
    signal dragExit(int zone)

    Rectangle {
        id: bench
        width: parent.width
        height: benchHeight
        color: benchDropArea.containsDrag ? Themes.dropHighlightColor : "grey"

        DropArea {
            id: benchDropArea
            anchors.fill: parent
            onEntered: pitchScheme.dragEnter(PitchZones.bench)
            onExited: pitchScheme.dragExit(PitchZones.bench)
        }

        Image {
            id: benchImg
            source: "qrc:/img/bench.png"
            height: benchHeight
            width: height
            anchors {
                left: parent.left
                leftMargin: Sizes.featuredStats.margin
                verticalCenter: parent.verticalCenter
            }
        }

        Grid {
            id: benchGrid
            rows: 2
            columns: (bench.width - benchImg.width - 2*Sizes.featuredStats.margin) / (benchSpacing + Sizes.playerHandleSize)
            spacing: benchSpacing
            anchors.left: benchImg.right
            anchors.leftMargin: Sizes.featuredStats.margin
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            Repeater {
                model: benchLength
                delegate: Rectangle {
                    id: benchSpots
                    height: Sizes.playerHandleSize
                    width: Sizes.playerHandleSize
                    color: "transparent"
                }
            }
        }
    }

    Rectangle {
        id: pitch
        width: parent.width
        height: pitchScheme.height - bench.height
        anchors {
            top: bench.bottom
            bottom: parent.bottom
        }

        Rectangle {
            id: pitchLeftHalf
            width: parent.width / 2
            height: parent.height
            color: leftDropArea.containsDrag ? Themes.dropHighlightColor : "green"
            border.width: 1
            border.color: "white"
            anchors {
                left: parent.left
                bottom: parent.bottom
            }

            DropArea {
                id: leftDropArea
                anchors.fill: parent
                onEntered: pitchScheme.dragEnter(PitchZones.leftHalf)
                onExited: pitchScheme.dragExit(PitchZones.leftHalf)
            }

            Text {
                text: "Home"
                color: theme.textColor
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                anchors {
                    top: parent.top
                    topMargin: Sizes.featuredStats.smallMargin
                    left: parent.left
                    leftMargin: Sizes.featuredStats.smallMargin
                }
            }
        }

        Rectangle {
            id: pitchRightHalf
            width: parent.width / 2
            height: parent.height
            color: rightDropArea.containsDrag ? Themes.dropHighlightColor : "green"
            border.width: 1
            border.color: "white"
            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            DropArea {
                id: rightDropArea
                anchors.fill: parent
                onEntered: pitchScheme.dragEnter(PitchZones.rightHalf)
                onExited: pitchScheme.dragExit(PitchZones.rightHalf)
            }

            Text {
                text: "Away"
                color: theme.textColor
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                anchors {
                    top: parent.top
                    topMargin: Sizes.featuredStats.smallMargin
                    right: parent.right
                    rightMargin: Sizes.featuredStats.smallMargin
                }
            }
        }

        Rectangle {
            id: pitchCenter
            height: parent.height * 3/4
            width: height
            radius: height / 2
            anchors.centerIn: parent
            color: centerDropArea.containsDrag ? Themes.dropHighlightColor : "green"
            border.width: 1
            border.color: "white"

            DropArea {
                id: centerDropArea
                anchors.fill: parent
                onEntered: pitchScheme.dragEnter(PitchZones.center)
                onExited: pitchScheme.dragExit(PitchZones.center)
            }
        }
    }
}
