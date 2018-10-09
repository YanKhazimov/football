import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"

Item {
    id: root
    height: parent.height
    width: Sizes.featuredStats.wingShare * parent.width
    property string img: ""
    property QtObject theme: null

    signal clicked()

    function update(show)
    {
        img.source = show ? root.img : ""
        overlay.visible = show
        mouseArea.enabled = show
    }

    Image {
        id: img
        anchors.centerIn: parent
        source: root.img
        width: Sizes.elementButtonSize.width
        height: Sizes.elementButtonSize.height
    }

    ColorOverlay {
        id: overlay
        anchors.fill: img
        source: img
        color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.highlightColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        propagateComposedEvents: true
    }
}
