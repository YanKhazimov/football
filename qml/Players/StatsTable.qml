import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/qml/visualStyles"

TableView {
    headerVisible: false
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    style: TableViewStyle {
        backgroundColor: "transparent"
    }

    itemDelegate: Item {
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "yellow"//color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
            text: styleData.value
            font.family: Themes.fontFamily
            font.pixelSize: 12
        }
    }
}
