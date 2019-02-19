import QtQuick 2.6
import QtQuick.Controls 2.1
import "qrc:/qml/visualStyles"

Column {
    spacing: Sizes.featuredStats.smallMargin

    property var theme: null

    signal buttonClicked(int idx)

    Repeater {
        model: ["^", "<>", "+"]
        delegate: Button {
            id: control
            height: Sizes.elementButtonSize.height
            width: Sizes.elementButtonSize.width
            //icon.source: "qrc:/img/cancel.png"
            hoverEnabled: true
            text: modelData
            contentItem: Text {
                text: control.text
                font.family: Themes.fontFamily
                color: theme.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: theme.primaryFillColor
                border.color: control.hovered ? theme.highlightColor : theme.secondaryFillColor
                border.width: Sizes.borderWidth
                radius: width/2
            }
            onClicked: buttonClicked(index)
        }
    }
}
