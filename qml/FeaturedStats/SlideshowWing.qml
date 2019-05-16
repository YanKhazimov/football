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

    signal clicked()

    function update(show)
    {
        image.source = show ? root.img : ""
        image.visible = show
        mouseArea.enabled = show
    }

    ColoredImage {
        id: image
        anchors.centerIn: parent
        width: Sizes.elementButtonSize.width
        height: Sizes.elementButtonSize.height
        source: root.img
        color: mouseArea.containsMouse ? theme.highlightColor : theme.secondaryColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        propagateComposedEvents: true
    }
}
