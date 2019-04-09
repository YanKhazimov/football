import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "qrc:/qml/visualStyles"

TableView {
    id: root
    property var theme: null

    Component.onCompleted: {
        selection.select(0)

        var column = getColumn(2)
        column.visible = true
    }

    function replaceColumn(statCategory) {
        for (var columnIdx = 2; columnIdx < columnCount; ++columnIdx)
        {
            var column = getColumn(columnIdx)
            column.visible = (column.role === statCategory)
        }
    }

    TableViewColumn {
        delegate: Text {
            text: model.index + 1
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        title: "#"
        width: root.width / 10
        resizable: false
        movable: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.PlayerName
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        //role: "Name"
        title: lang.playerColumn
        width: root.width * 6/10
        resizable: false
        movable: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Rating
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        role: lang.ratingColumn
        title: lang.ratingColumn
        width: root.width * 3/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.WinsLosses
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        role: lang.wdlColumn
        title: lang.wdlColumn
        width: root.width * 3/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Progress
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        role: lang.progressColumn
        title: lang.progressColumn
        width: root.width * 3/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Relevance
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        role: lang.relevanceColumn
        title: lang.relevanceColumn
        width: root.width * 3/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Dedication
            horizontalAlignment: Text.AlignHCenter
            font.family: Themes.fontFamily
            color: styleData.selected ? root.theme.textColor : root.theme.primaryFillColor
        }
        role: lang.dedicationColumn
        title: lang.dedicationColumn
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
            Rectangle {
                height: 1
                width: parent.width
                color: root.theme.textColor
                anchors.bottom: parent.bottom
            }
        }

        transientScrollBars: true
    }
}
