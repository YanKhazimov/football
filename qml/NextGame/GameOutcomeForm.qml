import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    color: Qt.rgba(0.8, 0.8, 0.8, 0.8)

    ColumnLayout {
        anchors.fill: parent
        spacing: Sizes.featuredStats.smallMargin

        Calendar {
            id: calendar
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: Sizes.featuredStats.smallMargin
            Layout.leftMargin: Sizes.featuredStats.smallMargin
            Layout.rightMargin: Sizes.featuredStats.smallMargin

            maximumDate: new Date(Date.now())
            locale: Qt.locale("en_US")
        }

        Row
        {
            spacing: Sizes.featuredStats.smallMargin
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: Sizes.featuredStats.smallMargin

            Button {
                id: cancelButton
                height: Sizes.elementButtonSize.height
                width: Sizes.elementButtonSize.width
                iconSource: "qrc:/img/cancel.png"
                onClicked: root.visible = false
            }

            SpinBox {
                id: homeScore
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                selectByMouse: true
                minimumValue: 0
                menu: null
            }

            SpinBox {
                id: awayScore
                font.family: Themes.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                selectByMouse: true
                minimumValue: 0
                menu: null
            }

            Button {
                readonly property bool active: gamesModel.isDateBusy(calendar.selectedDate) === false
                id: okButton
                iconSource: active ? "qrc:/img/ok.png" : ""
                isDefault: true
                enabled: active
                onClicked: {
                    gamesModel.addGame()
                    root.visible = false
                }
                height: Sizes.elementButtonSize.height
                width: Sizes.elementButtonSize.width
                TextEdit {
                    anchors.centerIn: parent
                    textFormat: TextEdit.RichText
                    readOnly: true
                    text: okButton.active ? "" : "Date<br/>busy"
                    font.family: Themes.fontFamily
                }
            }
        }
    }
}
