import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    property string name: ""
    property QtObject theme: null
    property int defaultWidth: 128
    readonly property real heightMultipilier: 1.5
    property bool selected: false

    signal clicked()

    function extend (animate) {
        selected = true

        if (animate === false)
        {
            widthBehavior.enabled = false
            xBehavior.enabled = false
        }

        x = parent.x//Math.max(parent.x, (parent.x + parent.width) - defaultWidth * heightMultipilier)
        width = parent.width + Sizes.borderWidth//Math.min(parent.width, defaultWidth * heightMultipilier)

        if (animate === false)
        {
            widthBehavior.enabled = true
            xBehavior.enabled = true
        }
    }

    function reset () {
        selected = false

        x = Math.max(parent.x, (parent.x + parent.width) - defaultWidth)
        width = Math.min(parent.width, defaultWidth) + Sizes.borderWidth
    }

    height: 32
    width: defaultWidth
    color: theme.primaryFillColor
    border.color: (mouseArea.containsMouse && !selected) ? theme.highlightColor : theme.secondaryFillColor
    border.width: Sizes.borderWidth

    Behavior on width {
        id: widthBehavior
        PropertyAnimation { duration: 300 }
    }
    Behavior on x {
        id: xBehavior
        PropertyAnimation { duration: 300 }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }

    Text {
        anchors.centerIn: parent
        text: root.name
        color: theme.textColor
        font.pixelSize: Sizes.fontPixelSize
        font.family: Themes.fontFamily
    }
}
