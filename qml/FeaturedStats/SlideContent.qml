import QtQuick 2.0
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"

Rectangle {
    id: contentRoot
    property string title: ""
    property string descr: ""
    property var statGroupsModel: null
    property var theme: null

    color: theme.primaryColor

    RowLayout {
        id: row
        spacing: Sizes.featuredStats.margin
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
                    color: theme.secondaryColor
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
                    color: theme.secondaryColor
                }
            }
        }
        GroupedImageRow {
            Layout.fillHeight: true
            Layout.fillWidth: true

            theme: contentRoot.theme
            statGroupsModel: contentRoot.statGroupsModel
        }
    }
}
