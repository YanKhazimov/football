import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"

TableView {
    id: root
    property QtObject theme: null
    headerVisible: false
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    TableViewColumn {
        id: headColumn
        delegate: Image {
            id: img
            source: "qrc:/img/menuarrow.png"
            visible: root.currentRow === model.index
            horizontalAlignment: Image.AlignRight
            fillMode: Image.PreserveAspectFit

            ColorOverlay {
                source: img
                anchors.fill: img
                color: theme.secondaryFillColor
            }
        }
        width: root.width / 10
    }

    TableViewColumn {
        delegate: Text {
            text: model.StatCategory + ":"
            horizontalAlignment: Text.AlignRight
            color: theme.textColor
            font.bold: root.currentRow === model.index
            font.pixelSize: 14
            font.family: Themes.fontFamily
        }
        width: (root.width - headColumn.width) * 1/2
    }

    TableViewColumn {
        delegate: Text {
            text: model.StatValue
            horizontalAlignment: Text.AlignHCenter
            color: theme.textColor
            font.bold: root.currentRow === model.index
            font.pixelSize: 14
            font.family: Themes.fontFamily
        }
        width: (root.width - headColumn.width) * 1/2
    }

    style: TableViewStyle {
        rowDelegate: Rectangle {
            color: root.theme.primaryFillColor
            height: Sizes.fontPixelSize
        }

        backgroundColor: root.theme.primaryFillColor
    }

    frameVisible: false
}
