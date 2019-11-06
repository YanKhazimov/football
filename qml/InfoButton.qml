import QtQuick 2.6
import QtQuick.Controls 2.1
import "qrc:/qml/visualStyles"

Rectangle {
    id: wave
    property int length: Sizes.elementButtonSize.width
    property real spreadMultiplier: 4
    property int times: 3
    property color waveColor: "transparent"
    property color borderColor: theme.primaryColor
    property color backgroundColor
    property bool runByDefault: true
    property var theme: null
    property bool firstTime: true

    signal clicked()

    width: length
    height: width
    color: "transparent"
    radius: width / 2
    border.color: borderColor
    border.width: length / 20

    Rectangle {
        anchors.centerIn: parent
        width: length
        height: width
        radius: width / 2
        color: theme.primaryColor
    }

    Connections {
        target: broadcaster
        onBroadcast: {
            if (receivers.indexOf(wave.objectName) !== -1 && firstTime)
            {
                wave.run(wave.times)
                wave.firstTime = false
            }
        }
    }

    function run(count) {
        console.log("run", count)
        times = count
        widthAnimation.running = true
        colorAnimation.running = true
    }

    PropertyAnimation {
        id: widthAnimation
        target: wave
        duration: 300 * spreadMultiplier
        property: "width"
        from: length
        to: length * spreadMultiplier
        loops: times
        running: wave.runByDefault
    }

    ColorAnimation on borderColor {
        id: colorAnimation
        duration: 300 * spreadMultiplier
        from: theme.secondaryColor
        to: "transparent"
        loops: times
        running: wave.runByDefault
        onStopped: borderColor = "transparent"
    }

    ColoredImage {
        anchors.centerIn: parent
        width: length
        height: width
        source: "qrc:/img/info.png"
        color: theme.secondaryColor
    }

    MouseArea {
        anchors.centerIn: parent
        width: length
        height: width
        hoverEnabled: true

        onClicked: wave.clicked()
    }
}

