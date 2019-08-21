import QtQuick 2.0
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Window 2.0
import "qrc:/qml/visualStyles"

QQC2.ApplicationWindow {
    id: window

    visible: true

    property bool updateFailed: false
    Connections {
        target: updater
        onError: updateFailed = true
    }

    signal closed()

    width: 300
    height: 200
    color: "black"

    Repeater {
        model: updater.messages
        delegate: Text {
            id: name
            text: modelData
            font.pointSize: 12
            color: "white"
            y: height * index
        }
    }

    Rectangle {
        width: 100
        height: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height
        visible: window.updateFailed
        color: okButton.containsMouse ? "lightgrey" : "white"

        MouseArea {
            id: okButton
            anchors.fill: parent
            hoverEnabled: true
            onClicked: closed()
        }

        Text {
            anchors.centerIn: parent
            text: "OK"
            font.pointSize: 12
        }
    }

    Image {
        id: waitImage
        source: "qrc:/img/wait.png"
        height: Sizes.elementButtonSize.height
        width: Sizes.elementButtonSize.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height
        anchors.horizontalCenter: parent.horizontalCenter
        visible: !window.updateFailed
    }
    PropertyAnimation {
        duration: 1000
        target: waitImage
        property: "rotation"
        from: 0
        to: 360
        loops: Animation.Infinite
        running: true
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
