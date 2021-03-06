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
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
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
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        //role: "Name"
        title: lang.playerColumn
        width: root.width * 5/10
        resizable: false
        movable: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Rating
            horizontalAlignment: Text.AlignHCenter
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        role: lang.ratingColumn
        title: lang.ratingColumn
        width: root.width * 4/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.WinsLosses
            horizontalAlignment: Text.AlignHCenter
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        role: lang.wdlColumn
        title: lang.wdlColumn
        width: root.width * 4/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Progress
            horizontalAlignment: Text.AlignHCenter
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        role: lang.progressColumn
        title: lang.progressColumn
        width: root.width * 4/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Relevance
            horizontalAlignment: Text.AlignHCenter
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        role: lang.relevanceColumn
        title: lang.relevanceColumn
        width: root.width * 4/10
        resizable: false
        visible: false
    }
    TableViewColumn {
        delegate: Text {
            text: model.Dedication
            horizontalAlignment: Text.AlignHCenter
            font.family: theme.fontFamily
            font.pixelSize: 14
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
        }
        role: lang.dedicationColumn
        title: lang.dedicationColumn
        width: root.width * 4/10
        resizable: false
        visible: false
    }

    itemDelegate: Item {
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: styleData.selected ? root.theme.secondaryColor : root.theme.primaryColor
            text: styleData.value.toString()
            font.family: theme.fontFamily
        }
    }

    style: TableViewStyle {
        rowDelegate: Rectangle {
            color: styleData.selected ? root.theme.highlightColor : root.theme.secondaryColor
            height: 20
            Rectangle {
                height: 1
                color: root.theme.primaryColor
                anchors { bottom: parent.top; left: parent.left; right: parent.right }
            }
        }

        backgroundColor: root.theme.primaryColor

        headerDelegate: Rectangle {
            height: textItem.implicitHeight * 1.2
            width: textItem.implicitWidth
            color: root.theme.primaryColor
            Text {
                id: textItem
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: styleData.value
                font.family: theme.fontFamily
                font.pixelSize: 16
                font.bold: true
                color: root.theme.secondaryColor
                renderType: Text.NativeRendering
            }
            Rectangle {
                height: 1
                width: parent.width
                color: root.theme.secondaryColor
                anchors.bottom: parent.bottom
            }
        }

        transientScrollBars: true
    }

    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
}
