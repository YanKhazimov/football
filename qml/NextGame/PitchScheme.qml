import QtQuick 2.0
import "."
import "qrc:/qml/visualStyles"

Rectangle {
    id: pitchScheme
    anchors.fill: parent

    property point pitchCenterCoords: Qt.point(pitchCenterArea.x + pitchCenterArea.width/2,
                                               bench.height + pitchCenterArea.y + pitchCenterArea.height/2)

    property int benchSpacing: Sizes.featuredStats.smallMargin / 2

    function benchPlayerZone(index) {

        var x = index % benchGrid.columns * (benchSpacing + Sizes.playerHandleWidth)
        var y = Math.floor(index / benchGrid.columns) * (benchSpacing + Sizes.playerHandleWidth)

        return Qt.point(benchGrid.x + x,
                        benchGrid.y + y)
    }

    property int benchLength: 0

    function calculatePosition(zone, idx, count) {
        if (zone === PitchZones.leftHalf) {
            return pitchLeftHalf.calculatePosition(idx, count)
        }
        else if (zone === PitchZones.center) {
            return pitchCenterArea.calculatePosition(idx, count)
        }
    }

    function showHint(zone, idx, count) {
        var position
        if (zone === PitchZones.leftHalf) {
            position = pitchLeftHalf.calculatePosition(idx, count)
        }
        else if (zone === PitchZones.center) {
            position = pitchCenterArea.calculatePosition(idx, count)
        }
        positionHint.x = position.x - positionHint.width/2
        positionHint.y = position.y - positionHint.height/2
        positionHint.visible = true
    }

    function hideHint() {
        positionHint.visible = false
    }

    signal dragEnter(int zone)
    signal dragExit(int zone)

    Rectangle {
        id: bench
        width: parent.width
        height: Sizes.playerHandleWidth + Sizes.fontPixelSize + 2 * Sizes.featuredStats.smallMargin
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
            height: parent.height
            width: height
            anchors {
                left: parent.left
                leftMargin: Sizes.featuredStats.margin
                verticalCenter: parent.verticalCenter
            }
        }

        Grid {
            id: benchGrid
            rows: 1//2
            columns: (bench.width - benchImg.width - 2*Sizes.featuredStats.margin) / (benchSpacing + Sizes.playerHandleWidth)
            spacing: benchSpacing
            anchors.left: benchImg.right
            anchors.leftMargin: Sizes.featuredStats.margin
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            Repeater {
                model: benchLength
                delegate: Rectangle {
                    id: benchSpots
                    height: Sizes.playerHandleWidth
                    width: Sizes.playerHandleWidth
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
            color: "green"
            border.width: 1
            border.color: "white"
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            property point pitchCenter: Qt.point(width, height/2)
            property real pitchCenterRadius: pitchCenterArea.width / 2
            property real preferredSliceAngle: Math.PI / 10 // should depend on handle size

            function calculatePosition(idx, count) {
                var startAngle = Math.PI - (count - 1) / 2 * preferredSliceAngle
                var angle = startAngle + idx * preferredSliceAngle
                var x = pitchCenter.x + 1.5 * pitchCenterRadius * Math.cos(angle)
                var y = pitchCenter.y - 1.5 * pitchCenterRadius * Math.sin(angle)
                return mapToItem(pitchScheme, x, y)
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
            id: pitchCenterArea
            height: parent.height - 2 * Sizes.featuredStats.margin
            width: height
            radius: height / 2
            anchors.centerIn: parent
            color: "green"
            border.width: 1
            border.color: "white"

            property point pitchCenter: Qt.point(width/2, height/2)
            property real pitchCenterRadius: pitchCenterArea.width / 2
            property real preferredSliceAngle: Math.PI * 2/10

            function calculatePosition(idx, count) {
                var startAngle = Math.PI / 2
                var angle = startAngle - idx * preferredSliceAngle
                var r = pitchCenterRadius - ((Sizes.playerHandleWidth + Sizes.fontPixelSize) / 2 + Sizes.featuredStats.smallMargin)
                var x = pitchCenter.x + r * Math.cos(angle)
                var y = pitchCenter.y - r * Math.sin(angle)
                return mapToItem(pitchScheme, x, y)
            }

            DropArea {
                id: centerDropArea
                anchors.fill: parent
                onEntered: pitchScheme.dragEnter(PitchZones.center)
                onExited: pitchScheme.dragExit(PitchZones.center)
            }
        }
    }

    Rectangle {
        id: positionHint
        width: Sizes.playerHandleWidth
        height: Sizes.playerHandleWidth
        radius: Sizes.playerHandleWidth
        visible: false
        color: Themes.dropHighlightColor
    }
}
