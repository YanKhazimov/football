import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    property string name: ""
    property var theme: null
    property int defaultWidth: 128 + 32
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
    color: theme.primaryColor
    border.color: (mouseArea.containsMouse && !selected) ? theme.secondaryColor : theme.highlightColor
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
        id: text
        anchors.centerIn: parent
        text: root.name
        color: theme.secondaryColor
        font.pixelSize: Sizes.fontPixelSize
        font.family: theme.fontFamily
        font.bold: root.selected
    }

    ColoredImage {
        source: "qrc:/img/lock.png"
        visible: !root.enabled
        anchors {
            right: parent.right
            rightMargin: Sizes.featuredStats.smallMargin / 2
            verticalCenter: parent.verticalCenter
        }
        height: parent.height/2
        width: height
        color: root.theme.secondaryColor
    }
}
