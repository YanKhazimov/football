import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"
import ".."

Item {
    id: root
    height: parent.height
    width: Sizes.featuredStats.wingShare * parent.width
    property string img: ""
    property var theme: null
    property bool available: true

    signal clicked()

    function update(show)
    {
        image.source = show ? root.img : ""
        image.visible = show
        mouseArea.enabled = show
    }

    Rectangle {
        id: imageBackground
        anchors.centerIn: parent
        width: Sizes.elementButtonSize.width
        height: Sizes.elementButtonSize.height
        radius: Sizes.elementButtonSize.height / 2
        color: root.theme.primaryColor
    }

    ColoredImage {
        id: image
        anchors.fill: imageBackground
        source: root.img
        color: (root.available && mouseArea.containsMouse) ? root.theme.highlightColor : root.theme.secondaryColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        propagateComposedEvents: true
    }
}
