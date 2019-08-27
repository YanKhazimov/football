import QtQuick 2.0
import QtQuick.Layouts 1.3
import com.abc.abclib 1.0
import "qrc:/qml/visualStyles"
import "../Players"

Rectangle {
    id: root
    color: "transparent"
    property var theme: null

    Rectangle {
        anchors.fill: parent
        anchors.margins: Sizes.smallMargin

        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.5; color: root.theme.primaryColor }
            GradientStop { position: 1.0; color: "transparent" }
        }

    ColumnLayout {
        anchors.fill: parent

        Text {
            text: new Date().getFullYear()
            Layout.alignment: Qt.AlignHCenter
            font.family: theme.fontFamily
            font.pointSize: 16
            font.bold: true
            color: root.theme.secondaryColor
        }

        Repeater {
            model: [[lang.progressColumn, 267, progressModel], [lang.dedicationColumn, 268, dedicationModel]]
            delegate: Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Sizes.margin
                Layout.alignment: Qt.AlignHCenter

                StatsTable {
                    property GlobalStatPresenter presenter: modelData[2]
                    model: presenter
                    anchors.fill: parent
                    theme: root.theme

                    Component.onCompleted: {
                        replaceColumn(modelData[0])
                        selection.deselect()
                    }
                }
            }
        }
    }
    }
}
