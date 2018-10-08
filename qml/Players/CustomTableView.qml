import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/qml/visualStyles"

TableView {
    id: root
    property QtObject theme: null
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
    //backgroundVisible: false

    itemDelegate: Item {
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
            text: styleData.value
            font.family: Themes.fontFamily
            font.pixelSize: 12
        }
    }

    style: TableViewStyle {
        rowDelegate: Rectangle {
            color: styleData.selected ? root.theme.secondaryFillColor : "white"
            radius: height
        }

        headerDelegate: Rectangle {
            height: textItem.implicitHeight * 1.2
            width: textItem.implicitWidth
            color: root.theme.primaryFillColor
            border.width: 1
            border.color: root.theme.textColor
            Text {
                id: textItem
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter//styleData.textAlignment
                text: styleData.value
                font.family: Themes.fontFamily
                font.pixelSize: 16
                font.bold: true
                color: root.theme.textColor
                renderType: Text.NativeRendering
            }
        }
    }
}
