import QtQuick 2.0
import QtGraphicalEffects 1.0
import "."
import "qrc:/qml/visualStyles"

Rectangle {
    id: pitchScheme
    anchors.fill: parent

    property QtObject theme: null
    property point pitchCenterCoords: Qt.point(pitchCenterArea.x + pitchCenterArea.width/2,
                                               bench.height + pitchCenterArea.y + pitchCenterArea.height/2)

    property int benchSpacing: Sizes.featuredStats.smallMargin / 2

    property var zoneShapes: [bench, pitchLeftHalf, pitchRightHalf, pitchCenterArea]

    property int benchLength: 0

    function calculatePosition(zone, idx, count) {
        return zoneShapes[zone].calculatePosition(idx, count)
    }

    function showHint(zone, idx, count, offset) {
        var position = zoneShapes[zone].calculatePosition(idx, count)
        positionHint.x = position.x - positionHint.width/2 + offset.x
        positionHint.y = position.y - positionHint.height/2 + offset.y
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
        height: Sizes.playerHandleWidth + Sizes.playerHandleRatingHeight + 2 * Sizes.featuredStats.smallMargin
        color: "grey"

        function calculatePosition(idx, count) {
            var x = benchImg.x + benchImg.width + Sizes.playerHandleWidth/2
                    + Sizes.featuredStats.smallMargin
                    + idx * (benchSpacing + Sizes.playerHandleWidth)
            var y = height / 2
            return mapToItem(pitchScheme, x, y)
        }

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
                leftMargin: Sizes.featuredStats.smallMargin
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle {
        id: pitch
        width: parent.width
        height: pitchScheme.height - bench.height
        anchors.bottom: parent.bottom

        Image {
            id: background
            source: "qrc:/img/bg.jpg"
            anchors.fill: parent
            fillMode: Image.TileHorizontally
        }

        ColorOverlay {
            source: background
            anchors.fill: background
            color: Qt.rgba(0.2, 0.2, 0.2, 0.7)
        }

        Rectangle {
            id: pitchLeftHalf
            width: parent.width / 2 + Sizes.borderWidth / 2
            height: parent.height
            color: "transparent"
            border.width: Sizes.borderWidth
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
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    top: parent.top
                }
                width: parent.width - pitchCenterArea.width/2

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
            width: parent.width / 2 + Sizes.borderWidth / 2
            height: parent.height
            color: "transparent"
            border.width: Sizes.borderWidth
            border.color: "white"
            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            property point pitchCenter: Qt.point(0, height/2)
            property real pitchCenterRadius: pitchCenterArea.width / 2

            //unite

            property real preferredSliceAngle: Math.PI / 10 // should depend on handle size

            function calculatePosition(idx, count) {
                var startAngle = (count - 1) / 2 * preferredSliceAngle
                var angle = startAngle - idx * preferredSliceAngle
                var x = pitchCenter.x + 1.5 * pitchCenterRadius * Math.cos(angle)
                var y = pitchCenter.y - 1.5 * pitchCenterRadius * Math.sin(angle)
                return mapToItem(pitchScheme, x, y)
            }

            DropArea {
                id: rightDropArea
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    top: parent.top
                }
                width: parent.width - pitchCenterArea.width/2

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
            color: "transparent"
            border.width: Sizes.borderWidth
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
        color: "transparent"
        border.width: Sizes.borderWidth
        border.color: theme.secondaryFillColor

        Rectangle {
            id: rating
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: Sizes.playerHandleRatingHeight
            radius: height / 4
            color: "transparent"
            border.width: Sizes.borderWidth
            border.color: theme.secondaryFillColor
        }
    }
}
