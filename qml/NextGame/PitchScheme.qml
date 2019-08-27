import QtQuick 2.0
import QtGraphicalEffects 1.0
import "."
import ".."
import "qrc:/qml/visualStyles"

Rectangle {
    id: pitchScheme
    anchors.fill: parent
    anchors.margins: Sizes.borderWidth

    property var theme: null
    property point pitchCenterCoords: Qt.point(pitchCenterArea.x + pitchCenterArea.width/2,
                                               bench.height + pitchCenterArea.y + pitchCenterArea.height/2)

    property real teamAngleDiff: Math.PI / 10 // should depend on handle size
    property real noTeamAngleDiff: Math.PI * 2/10
    property real pitchCenterRadius: pitchCenterArea.width / 2

    property int benchSpacing: Sizes.smallMargin / 2

    property var zoneShapes: [bench, pitchLeftHalf, pitchRightHalf, pitchCenterArea]
    property int homeDiff: 0
    property int awayDiff: 0

    property int benchLength: 0

    function signed(number) {
        return (number > 0) ? ("+" + number) : number
    }

    function getBenchCapacity() {
        return Math.floor(bench.getCapacity())
    }

    function getBenchPlayerSpace() {
        return benchSpacing + Sizes.playerHandleWidth
    }

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
    signal benchScrolled(int offset)

    property int benchHeight: bench.height

    Rectangle {
        id: bench
        width: parent.width
        height: Sizes.playerHandleWidth + Sizes.playerHandleRatingHeight + 2 * Sizes.smallMargin

        function calculatePosition(idx, count) {
            var x = benchImg.x + benchImg.width + Sizes.smallMargin
                    + leftArrow.width + Sizes.smallMargin
                    + Sizes.playerHandleWidth / 2
                    + idx * (benchSpacing + Sizes.playerHandleWidth)
            var y = height / 2
            return mapToItem(pitchScheme, x, y)
        }

        function getCapacity() {
            var playersSpace = width - (benchImg.x + benchImg.width + 2 * Sizes.smallMargin
                                        + leftArrow.width + rightArrow.width + 2 * Sizes.smallMargin)
            var capacity = playersSpace / (benchSpacing + Sizes.playerHandleWidth)
            if (playersSpace % (benchSpacing + Sizes.playerHandleWidth) >= Sizes.playerHandleWidth)
                ++capacity
            return capacity
        }

        ColoredImage {
            source: "qrc:/img/bg.jpg"
            anchors.fill: parent
            color: Qt.rgba(0.2, 0.2, 0.2, 0.7)
        }

        DropArea {
            id: benchDropArea
            anchors.fill: parent
            onEntered: pitchScheme.dragEnter(PitchZones.bench)
            onExited: pitchScheme.dragExit(PitchZones.bench)
        }

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            hoverEnabled: true
            onWheel: {
                if ((wheel.buttons & Qt.LeftButton) == 0)
                {
                    benchScrolled(wheel.angleDelta.y / 120)
                }
            }
        }

        Image {
            id: benchImg
            source: "qrc:/img/bench.png"
            height: parent.height
            width: height
            anchors {
                left: parent.left
                leftMargin: Sizes.smallMargin
                verticalCenter: parent.verticalCenter
            }
        }

        ColoredImage {
            id: leftArrow
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            anchors {
                left: benchImg.right
                leftMargin: Sizes.smallMargin
                verticalCenter: parent.verticalCenter
            }
            source: "qrc:/img/l.png"
            color: leftArrowArea.containsMouse ? pitchScheme.theme.highlightColor : pitchScheme.theme.secondaryColor

            MouseArea {
                id: leftArrowArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: benchScrolled(1)
            }
        }

        ColoredImage {
            id: rightArrow
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            anchors {
                right: parent.right
                rightMargin: Sizes.smallMargin
                verticalCenter: parent.verticalCenter
            }
            source: "qrc:/img/r.png"
            color: rightArrowArea.containsMouse ? pitchScheme.theme.highlightColor : pitchScheme.theme.secondaryColor

            MouseArea {
                id: rightArrowArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: benchScrolled(-1)
            }
        }
    }

    Rectangle {
        id: pitch
        width: parent.width
        height: pitchScheme.height - bench.height
        anchors.bottom: parent.bottom

        ColoredImage {
            id: background
            source: "qrc:/img/bg.jpg"
            anchors.fill: parent
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

            function calculatePosition(idx, count) {
                var startAngle = Math.PI - (count - 1) / 2 * teamAngleDiff
                var angle = startAngle + idx * teamAngleDiff
                var pitchCenter = Qt.point(width, height/2)
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

            ColoredImage {
                id: homeShirt
                source: "qrc:/img/shirt.png"
                width: parent.width / 6
                height: width
                color: "white"
                anchors {
                    top: parent.top
                    topMargin: Sizes.smallMargin
                    left: parent.left
                    leftMargin: Sizes.smallMargin
                }

                Text {
                    text: lang.home
                    color: "white"
                    font.family: theme.fontFamily
                    anchors.centerIn: parent
                }
            }

            Text {
                anchors {
                    top: homeShirt.bottom
                    horizontalCenter: homeShirt.horizontalCenter
                }

                text: signed(homeDiff)
                color: "white"
                font.family: theme.fontFamily
                font.bold: true
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

            function calculatePosition(idx, count) {
                var startAngle = (count - 1) / 2 * teamAngleDiff
                var angle = startAngle - idx * teamAngleDiff
                var pitchCenter = Qt.point(0, height/2)
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

            ColoredImage {
                id: awayShirt
                source: "qrc:/img/shirt.png"
                width: parent.width / 6
                height: width
                color: "white"
                anchors {
                    top: parent.top
                    topMargin: Sizes.smallMargin
                    right: parent.right
                    rightMargin: Sizes.smallMargin
                }

                Text {
                    text: lang.away
                    color: "white"
                    font.family: theme.fontFamily
                    anchors.centerIn: parent
                }
            }

            Text {
                anchors {
                    top: awayShirt.bottom
                    horizontalCenter: awayShirt.horizontalCenter
                }

                text: signed(awayDiff)
                color: "white"
                font.family: theme.fontFamily
                font.bold: true
            }
        }

        Rectangle {
            id: pitchCenterArea
            height: parent.height - 2 * Sizes.margin
            width: height
            radius: height / 2
            anchors.centerIn: parent
            color: "transparent"
            border.width: Sizes.borderWidth
            border.color: "white"

            function calculatePosition(idx, count) {
                var startAngle = Math.PI / 2
                var angle = startAngle - idx * noTeamAngleDiff
                var pitchCenter = Qt.point(width/2, height/2)
                var r = pitchCenterRadius - ((Sizes.playerHandleWidth + Sizes.playerHandleRatingHeight) / 2 + Sizes.smallMargin)
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
        radius: Sizes.playerHandleWidth/3
        visible: false
        color: "transparent"
        border.width: Sizes.borderWidth
        border.color: theme.highlightColor

        Rectangle {
            id: rating
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: Sizes.playerHandleRatingHeight
            radius: height / 4
            color: "transparent"
            border.width: Sizes.borderWidth
            border.color: theme.highlightColor
        }
    }
}
