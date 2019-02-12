import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"
import ".."

TableView {
    id: root
    property var theme: null
    headerVisible: false
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
    frameVisible: false

    Component.onCompleted: {
        selection.select(0)
        currentRow = 0
    }

    TableViewColumn {
        id: headColumn
        horizontalAlignment: Text.AlignRight
        delegate: ColoredImage {
            source: "qrc:/img/menuarrow.png"
            visible: root.currentRow === model.index
            fillMode: Image.PreserveAspectFit
            color: theme.secondaryFillColor
        }
        width: root.width / 10
    }

    TableViewColumn {
        delegate: Text {
            text: model.StatCategory + ":"
            horizontalAlignment: Text.AlignRight
            color: styleData.selected ? theme.secondaryFillColor : theme.textColor
            font.bold: styleData.selected
            font.pixelSize: 14
            font.family: Themes.fontFamily
        }
        width: (root.width - headColumn.width) * 1/2
    }

    TableViewColumn {
        delegate: Text {
            text: model.StatValue === undefined ? "" : model.StatValue
            horizontalAlignment: Text.AlignHCenter
            color: styleData.selected ? theme.secondaryFillColor : theme.textColor
            font.bold: styleData.selected
            font.pixelSize: 14
            font.family: Themes.fontFamily
        }
        width: (root.width - headColumn.width) * 1/2
    }

    style: TableViewStyle {
        rowDelegate: Rectangle {
            color: /*styleData.pressed ? "red" :*/ "transparent"
            height: Sizes.fontPixelSize
        }

        backgroundColor: "transparent"
    }
}
