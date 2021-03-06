import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    color: Qt.rgba(0.8, 0.8, 0.8, 0.8)
    property var homeTeam: []
    property var awayTeam: []
    signal gameAdded()

    ColumnLayout {
        anchors.fill: parent
        spacing: Sizes.smallMargin

        Calendar {
            id: calendar
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: Sizes.smallMargin
            Layout.leftMargin: Sizes.smallMargin
            Layout.rightMargin: Sizes.smallMargin

            maximumDate: new Date(Date.now())
            locale: Qt.locale("en_US")
        }

        Row
        {
            spacing: Sizes.smallMargin
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: Sizes.smallMargin

            Button {
                id: cancelButton
                height: Sizes.elementButtonSize.height
                width: Sizes.elementButtonSize.width
                iconSource: "qrc:/img/cancel.png"
                onClicked: root.visible = false
            }

            SpinBox {
                id: homeScore
                font.family: theme.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                selectByMouse: true
                minimumValue: 0
                menu: null
            }

            SpinBox {
                id: awayScore
                font.family: theme.fontFamily
                font.pixelSize: Sizes.fontPixelSize
                selectByMouse: true
                minimumValue: 0
                menu: null
            }

            Button {
                id: okButton
                property bool active: gamesModel.isDateBusy(calendar.selectedDate) === false
                iconSource: active ? "qrc:/img/ok.png" : ""
                isDefault: true
                enabled: active
                onClicked: {
                    gamesModel.addGame(calendar.selectedDate, homeTeam, homeScore.value, awayTeam, awayScore.value)
                    gameAdded()
                    active = gamesModel.isDateBusy(calendar.selectedDate) === false
                    root.visible = false
                }
                height: Sizes.elementButtonSize.height
                width: Sizes.elementButtonSize.width
                TextEdit {
                    anchors.centerIn: parent
                    textFormat: TextEdit.RichText
                    readOnly: true
                    text: okButton.active ? "" : lang.dateBusy
                    font.family: theme.fontFamily
                }
            }
        }
    }
}
