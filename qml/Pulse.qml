import QtQuick 2.0
import QtQuick.Controls 2.12 as QQC2
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    implicitWidth: 200
    implicitHeight: 30
    property real cycleWidth: 200.0
    clip: true
    property var theme: null
    color: theme.primaryColor

    property var pulseModel: gamesModel.getPulse()

    Connections {
        target: gamesModel
        onPulseChanged: pulseModel = gamesModel.getPulse()
    }

    Repeater {
        model: 2
        delegate: Rectangle {
            id: delegate
            width: root.width
            anchors.top: root.top
            anchors.bottom: root.bottom
            color: "transparent"

            PropertyAnimation {
                target: delegate
                property: "x"
                to: 0 - index * root.width
                from: root.width - index * root.width
                duration: 10000
                loops: Animation.Infinite
                running: true
            }

            Canvas {
                id: canvas
                Connections {
                    target: root
                    onThemeChanged: canvas.requestPaint()
                }

                anchors.fill: parent

                property var model: root.pulseModel

                onPaint: {
                    var ctxt = getContext('2d')
                    ctxt.save()
                    ctxt.lineWidth = 1;
                    ctxt.strokeStyle = theme.secondaryColor
                    ctxt.beginPath();
                    var midY = parent.height / 2
                    var x = 0
                    var y = midY
                    ctxt.moveTo(x, y)

                    while(x < width)
                    {
                        for(var i = 0; i < model.length && x < width; i++)
                        {
                            var yCatet = root.height * 0.5 * model[i].y / 100
                            var xCatet = yCatet / 3

                            var preX = root.cycleWidth * model[i].x / 100 - yCatet / 2
                            if (preX > 0)
                            {
                                x += preX
                                ctxt.lineTo(x, y)
                            }

                            x += xCatet
                            y = midY - yCatet
                            ctxt.lineTo(x, y)
                            x += xCatet
                            y = midY + yCatet
                            ctxt.lineTo(x, y)
                            x += xCatet
                            y = midY
                            ctxt.lineTo(x, y)
                        }
                        if (model.length > 0 && x < width)
                        {
                            var postX = root.cycleWidth * (100 - model[model.length - 1].x) / 100
                            x += postX
                            ctxt.lineTo(x, y)
                        }
                    }

                    ctxt.lineTo(x + 10, midY)
                    ctxt.stroke();
                }
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: headerText.width + 2 * Sizes.smallMargin
        height: parent.height
        color: root.theme.primaryColor

        Text {
            id: headerText
            anchors.centerIn: parent
            text: lang.pulseStats.arg(root.pulseModel.length).arg(gamesModel.pulseRosterConsistency)
            color: root.theme.secondaryColor
            font.family: root.theme.fontFamily
            font.pixelSize: Sizes.fontPixelSize
            font.bold: true
        }
    }
}
