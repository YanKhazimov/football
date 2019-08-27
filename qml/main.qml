import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Window 2.0
import com.abc.abclib 1.0
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"
import "./FeaturedStats"
import "."

//https://www.futhead.com/icons/?page=1
//https://www.iconfinder.com/icons/2203522/arrow_botton_next_right_icon

QQC2.ApplicationWindow {
    id: window

    visible: true

    width: 1280
    height: 720

    property var theme: null

    Component.onCompleted: {
        theme = Themes.themes[config.value("theme")] // this seems to initialize the singleton
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }

    minimumHeight: featuredStats.height + contents.height
    minimumWidth: width

    maximumHeight: height
    maximumWidth: width

    Pulse {
        id: pulse
        height: 30
        width: window.width
        theme: window.theme
    }

    Slideshow {
        id: featuredStats
        anchors { top: pulse.bottom; horizontalCenter: parent.horizontalCenter }
        width: window.width

        title: lang.featuredStats
        theme: window.theme

        model: featuredStatsModel
    }

    CustomTabView {
        id: contents
        anchors { top: featuredStats.bottom }
        height: window.height - featuredStats.height - pulse.height
        width: window.width

        theme: window.theme
    }

    Rectangle {
        width: 2 * (Sizes.elementButtonSize.width + 2 * Sizes.margin)
        height: 5 * Sizes.elementButtonSize.height + (5 + 1) * Sizes.smallMargin
        radius: Sizes.elementButtonSize.height / 2
        anchors {
            bottom: parent.bottom
            bottomMargin: Sizes.margin / 2
            left: parent.left
            leftMargin: -width / 2
        }
        color: window.theme.primaryColor
        border.color: window.theme.highlightColor
        border.width: Sizes.borderWidth
    }

    Column {
        anchors {
            bottom: parent.bottom
            bottomMargin: Sizes.margin
            left: parent.left
            leftMargin: Sizes.margin
        }
        spacing: Sizes.smallMargin

        Image {
            id: refresher
            source: "qrc:/img/refresh.png"
            width: Sizes.elementButtonSize.width
            height: Sizes.elementButtonSize.height
            antialiasing: true
            MouseArea {
                id: refresherMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    refresher.source = "qrc:/img/wait.png"
                    rotationAnimation.running = true
                    syncManager.update()
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

            QQC2.ToolTip.delay: 100
            QQC2.ToolTip.timeout: 3000
            QQC2.ToolTip.visible: refresherMouseArea.containsMouse
            QQC2.ToolTip.text: lang.getText("updateData")
        }
        Connections {
            target: syncManager
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
                checked = false//true
                sortingStatModel.setFilter(checked)
            }
            onClicked: sortingStatModel.setFilter(checked)

            MouseArea {
                id: relevanceMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    relevanceCheckBox.checked = !relevanceCheckBox.checked
                    sortingStatModel.setFilter(relevanceCheckBox.checked)
                }
            }

            QQC2.ToolTip.delay: 100
            QQC2.ToolTip.timeout: 3000
            QQC2.ToolTip.visible: relevanceMouseArea.containsMouse
            QQC2.ToolTip.text: lang.getText("switchRelevance")
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
//                GradientStop { position: 0.0; color: theme.primaryColor }
//                GradientStop { position: 0.5; color: mouseArea.containsMouse ? theme.highlightColor : theme.primaryColor }
//                GradientStop { position: 1.0; color: theme.primaryColor }
//            }
            Image {
                id: languageSwitcher
                source: "qrc:/img/lang_" + lang.lang + ".png"
                anchors.centerIn: parent
                width: Sizes.elementButtonSize.width
                height: Sizes.elementButtonSize.height
                antialiasing: true
                MouseArea {
                    id: languageMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {//lang.setNext()
                        config.set("lang", lang.getNext())
                        languageSwitcher.source = "qrc:/img/lang_" + lang.getNext() + ".png"
                        popup.open()
                    }
                }

                QQC2.ToolTip.delay: 100
                QQC2.ToolTip.timeout: 3000
                QQC2.ToolTip.visible: languageMouseArea.containsMouse
                QQC2.ToolTip.text: lang.getText("changeLanguage")
            }
//            border.color: mouseArea.containsMouse ? theme.highlightColor : "transparent"
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
        width: warningText.contentWidth + 2 * Sizes.margin
        height: warningText.contentHeight + 2 * Sizes.margin
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
        width: warningText.contentWidth + 2 * Sizes.margin
        height: warningText.contentHeight + 2 * Sizes.margin
        modal: true
        focus: true
        closePolicy: QQC2.Popup.CloseOnEscape | QQC2.Popup.CloseOnReleaseOutside

        Text {
            id: updatePopupText
            text: ""
            anchors.centerIn: parent
        }
    }
}
