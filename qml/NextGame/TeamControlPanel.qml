import QtQuick 2.6
import QtQuick.Controls 2.1
import "qrc:/qml/visualStyles"

Column {
    spacing: Sizes.smallMargin

    property var theme: null
    property string splitText

    signal buttonClicked(int idx)

    Repeater {
        model: [lang.benchAll, splitText, lang.saveGame]
        delegate: Button {
            id: control
            height: Sizes.elementButtonSize.height
            width: Sizes.elementButtonSize.width * 3
            hoverEnabled: true
            text: modelData
            contentItem: Text {
                text: control.text
                font.family: theme.fontFamily
                color: theme.secondaryColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: theme.primaryColor
                border.color: control.hovered ? theme.secondaryColor : theme.highlightColor
                border.width: Sizes.borderWidth
                radius: width / 2
            }
            onClicked: buttonClicked(index)
        }
    }
}
