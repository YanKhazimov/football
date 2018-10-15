import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/qml/visualStyles"

TableView {
    id: root
    property QtObject theme: null
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    sortIndicatorVisible: true
    sortIndicatorColumn: 2
    sortIndicatorOrder: Qt.DescendingOrder

    Component.onCompleted: {
        selection.select(0)
    }

    function replaceColumn(statCategory) {
        var removedColumn
        for (var columnIdx = 2; columnIdx < columnCount; ++columnIdx)
        {
            var column = getColumn(columnIdx)

            column.visible = (column.role === statCategory)
//            console.log(column.role)
//            if (column.role === statCategory)
//            {
//                console.log("=")
//                removedColumn = column
//                break
//            }
        }
    }

    TableViewColumn {
        role: "rank"
        title: "#"
        width: root.width / 10
        resizable: false
        movable: false
    }
    TableViewColumn {
        role: "Name"
        title: "Player"
        width: root.width * 6/10
        resizable: false
    }
    TableViewColumn {
        role: "Rating"
        title: "Rating"
        width: root.width * 3/10
        resizable: false
    }
    TableViewColumn {
        role: "Wins_Losses"
        title: "W/L"
        width: root.width * 3/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        role: "Reliability"
        title: "Reliability"
        width: root.width * 3/10
        resizable: false
        visible: false
    }

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

        backgroundColor: root.theme.primaryFillColor

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
                horizontalAlignment: Text.AlignHCenter
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
