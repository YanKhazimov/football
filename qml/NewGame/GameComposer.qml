import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root

    anchors.fill: parent

    Image {
        anchors.fill: parent
        source: "qrc:/img/method-draw-image.svg"
    }

    Image {
        source: "qrc:/img/bench.ico"
        width: 100
        height: 100
    }

    Rectangle {
        radius: 32
        width: 64; height: 64
        y: 100
        color: "yellow"

        DropArea {
            id: dragTarget
//            width: 64; height: 64
//            y: 100
            anchors.fill: parent
            onContainsDragChanged: console.log(containsDrag)
            onEntered: console.log("entered")

            //keys: [ colorKey ]

//            Rectangle {
//                id: dropRectangle

//                anchors.fill: parent
//                color: "yellow"

//                states: [
//                    State {
//                        when: dragTarget.containsDrag
//                        PropertyChanges {
//                            target: dropRectangle
//                            color: "green"
//                        }
//                    }
//                ]
//            }
        }
    }

    Row {
        Repeater {
            model: [["p1", 1111], ["p2", 2222], ["p3", 3333], ["p3", 3333], ["p3", 3333],
                ["p3", 3333], ["p3", 3333], ["p3", 3333], ["p3", 3333], ["p3", 3333]]

            delegate: Item {
                id: del
                width: 40
                height: 40
                Drag.active: mouseArea.drag.active
                Rectangle {
                    color: "lightblue"
                    anchors.fill: parent
                }
                Text {
                    id: pic
                    text: modelData[0]
                }
                Text {
                    anchors.top: pic.bottom
                    text: modelData[1]
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    drag.target: parent
                }
            }
        }
    }
}
