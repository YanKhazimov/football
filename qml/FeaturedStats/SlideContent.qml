import QtQuick 2.0
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"

Rectangle {
    id: contentRoot
    property string title: "null"
    property string descr: "null"
    property var imgModel: null
    property QtObject theme: null

    color: theme.primaryFillColor

    RowLayout {
        id: row
        spacing: Sizes.featuredStats.smallMargin
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: Sizes.featuredStats.smallMargin
            bottom: parent.bottom
            bottomMargin: Sizes.featuredStats.smallMargin
        }
        Item {
            id: titleRect
            Layout.preferredWidth: contentRoot.width / 3
            Layout.minimumWidth: Math.max(statTitle.contentWidth, statDescr.contentWidth)
            Layout.maximumWidth: contentRoot.width / 2

            Layout.fillHeight: true
            ColumnLayout {
                id: cl
                anchors.centerIn: parent
                Text {
                    id: statTitle
                    Layout.alignment: Qt.AlignHCenter
                    text: contentRoot.title
                    color: theme.textColor
                    font.family: Themes.fontFamily
                    font.pointSize: 16
                    font.bold: true
                }
                Text {
                    id: statDescr
                    Layout.alignment: Qt.AlignHCenter
                    text: contentRoot.descr
                    font.family: Themes.fontFamily
                    font.pointSize: 14
                    color: theme.textColor
                }
            }
        }
        Item {
            id: flowRect
            Layout.fillHeight: true
            Layout.fillWidth: true

            Flow {
                id: flow
                anchors.centerIn: flowRect
                Repeater {
                    model: contentRoot.imgModel
                    delegate: Image {
                        source: model.modelData.player.photo
                        width: Math.min(Sizes.featuredStats.iconWidth,
                                        flowRect.width / contentRoot.imgModel.length)
                        height: width

                        MouseArea {
                            id: photoMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                        Text {
                            id: playerTooltip
                            anchors.bottom: parent.top
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: theme.textColor
                            text: model.modelData.player.name
                            visible: photoMouseArea.containsMouse
                            font.family: Themes.fontFamily
                            font.pixelSize: Sizes.fontPixelSize
                        }
                        Text {
                            id: statValue
                            anchors.top: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: theme.textColor
                            text: model.modelData.statValue
                            font.family: Themes.fontFamily
                            font.pixelSize: Sizes.fontPixelSize
                            font.bold: true
                        }
                    }
                }
            }
        }
    }

    Grid
    {
        id: debugGrid
        visible: false//true
        anchors.fill: parent
        z: 1
        readonly property int size: 20
        columns: Math.ceil(width / debugGrid.size)
        rows: Math.ceil(height / debugGrid.size)
        Repeater
        {
            model: parent.visible ? parent.rows : 0
            Repeater
            {
                id: columnRepeater
                property bool evenRowIndex: index % 2 === 0
                model: debugGrid.visible ? debugGrid.columns : 0
                Rectangle
                {
                    color: "red"
                    width: debugGrid.size
                    height: debugGrid.size

                    property bool evenColumnIndex: index % 2 === 0

                    opacity:
                    {
                        if (columnRepeater.evenRowIndex)
                        {
                            return evenColumnIndex ? 0.4 : 0.2
                        }
                        return evenColumnIndex ? 0.2 : 0
                    }
                }
            }
        }
    }


}
