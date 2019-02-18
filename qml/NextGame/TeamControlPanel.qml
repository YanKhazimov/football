import QtQuick 2.0
import QtQuick.Controls 1.4
import "qrc:/qml/visualStyles"

Column {
    spacing: Sizes.featuredStats.smallMargin

    signal benchAllClicked()
    signal splitClicked()
    signal addGameClicked()

    Button {
        id: benchAllButton
        height: Sizes.elementButtonSize.height
        width: Sizes.elementButtonSize.width
        //iconSource: "qrc:/img/cancel.png"
        text: "^"
        onClicked: benchAllClicked()
    }
    Button {
        id: splitButton
        height: Sizes.elementButtonSize.height
        width: Sizes.elementButtonSize.width
        text: "<>"
        onClicked: splitClicked()
    }

    Button {
        id: regGameButton
        height: Sizes.elementButtonSize.height
        width: Sizes.elementButtonSize.width
        //iconSource: "qrc:/img/cancel.png"
        text: "+"
        onClicked: addGameClicked()
    }
}
