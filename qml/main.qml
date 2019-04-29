import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Window 2.0
import com.abc.abclib 1.0
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"
import "."

//https://www.futhead.com/icons/?page=1
//https://www.iconfinder.com/icons/2203522/arrow_botton_next_right_icon

QQC2.ApplicationWindow {
    id: window

    visible: true

    width: 800
    height: 600
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }

    minimumHeight: featuredStats.height + contents.height
    minimumWidth: 600//featuredStats.minWidth

    maximumHeight: height
    maximumWidth: width

    property var theme: null

    Slideshow {
        id: featuredStats
        anchors { top: window.top; horizontalCenter: parent.horizontalCenter }
        width: window.width

        title: lang.featuredStats
        theme: window.theme

        model: featuredStatsModel
    }

    CustomTabView {
        id: contents
        anchors { top: featuredStats.bottom }
        height: window.height - featuredStats.height
        width: window.width

        theme: window.theme
    }

    Column {
        anchors {
            bottom: parent.bottom
            bottomMargin: Sizes.featuredStats.margin
            left: parent.left
            leftMargin: Sizes.featuredStats.margin
        }
        spacing: Sizes.featuredStats.smallMargin

        Image {
            id: refresher
            source: "qrc:/img/refresh.png"
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            antialiasing: true
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    refresher.source = "qrc:/img/wait.png"
                    rotationAnimation.running = true
                    updater.update()
                }
            }
            PropertyAnimation {
                id: rotationAnimation
                duration: 1000
                target: refresher
                property: "rotation"
                from: 0
                to: 360
                loops: 10
            }
        }
        Connections {
            target: updater
            onUpdateFinished: {
                updatePopupText.text = message
                updatePopup.open()
                refresher.source = "qrc:/img/refresh.png"
                rotationAnimation.running = false
            }
        }

        CheckBox {
            id: relevanceCheckBox
            style: CheckBoxStyle {
                indicator: Rectangle {
                    width: Sizes.elementButtonSize.width
                    height: Sizes.elementButtonSize.height
                    Image {
                        source: "qrc:/img/phil.jpg"
                        anchors.fill: parent
                    }
                    Image {
                        source: "qrc:/img/cancel.png"
                        anchors.fill: parent
                        visible: control.checked
                    }
                }
            }
            Component.onCompleted: {
                checked = true
                sortingStatModel.setFilter(checked)
            }
            onClicked: sortingStatModel.setFilter(checked)
        }

        Seasoner {
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            model: gamesModel.getSeasons()
        }

        Rectangle{
            width: Sizes.elementButtonSize.width// * 2
            height: Sizes.elementButtonSize.height// * 2
            radius: width/2
            color: "transparent"
            //gradient: Gradient {
//                GradientStop { position: 0.0; color: theme.primaryFillColor }
//                GradientStop { position: 0.5; color: mouseArea.containsMouse ? theme.secondaryFillColor : theme.primaryFillColor }
//                GradientStop { position: 1.0; color: theme.primaryFillColor }
//            }
            Image {
                id: languageSwitcher
                source: "qrc:/img/lang_" + lang.lang + ".png"
                anchors.centerIn: parent
                width: Sizes.elementButtonSize.width
                height: Sizes.elementButtonSize.height
                antialiasing: true
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {//lang.setNext()
                        config.set("lang", lang.getNext())
                        languageSwitcher.source = "qrc:/img/lang_" + lang.getNext() + ".png"
                        popup.open()
                    }
                }
            }
//            border.color: mouseArea.containsMouse ? theme.secondaryFillColor : "transparent"
//            border.width: Sizes.borderWidth
        }

        ThemeSwitcher {
            id: themeSwitcher
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            onThemeChanged: window.theme = theme
        }
    }

    QQC2.Popup {
        id: popup
        anchors.centerIn: parent
        width: warningText.contentWidth + 2 * Sizes.featuredStats.margin
        height: warningText.contentHeight + 2 * Sizes.featuredStats.margin
        modal: true
        focus: true
        closePolicy: QQC2.Popup.CloseOnEscape | QQC2.Popup.CloseOnReleaseOutside

        Text {
            id: warningText
            text: lang.languageChangeWarning
            anchors.centerIn: parent
        }
    }

    QQC2.Popup {
        id: updatePopup
        anchors.centerIn: parent
        width: warningText.contentWidth + 2 * Sizes.featuredStats.margin
        height: warningText.contentHeight + 2 * Sizes.featuredStats.margin
        modal: true
        focus: true
        closePolicy: QQC2.Popup.CloseOnEscape | QQC2.Popup.CloseOnReleaseOutside

        Text {
            id: updatePopupText
            text: ""
            anchors.centerIn: parent
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
