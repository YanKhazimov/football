import QtQuick 2.0
import "."
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    anchors.fill: parent

    property var allPlayersModel: null
    property var zoneModels: []

    onAllPlayersModelChanged: {
        var startingModel = []
        for (var i = 0; i < allPlayersModel.length; ++i)
        {
            if (allPlayersModel.getPlayer(i) !== null)
                startingModel.push(allPlayersModel.getPlayer(i).name)
        }
        zoneModels = [ startingModel, [], [], [] ]
    }

    PitchScheme {
        id: scheme
        benchLength: allPlayersModel.length

        onDragEnter: {
            if (dragInfo.sender === -1)
                dragInfo.sender = zone

            dragInfo.reciever = zone
        }

        onDragExit: {
            if (dragInfo.reciever === zone)
                dragInfo.reciever = -1
        }
    }

    Image {
        width: Sizes.elementButtonSize.width
        height: Sizes.elementButtonSize.height
        x: scheme.pitchCenterCoords.x - width/2
        y: scheme.pitchCenterCoords.y - height/2
        source: "qrc:/img/ball.png"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                console.log("zoneModels[PitchZones.bench]", zoneModels[PitchZones.bench])
                console.log("zoneModels[PitchZones.leftHalf]", zoneModels[PitchZones.leftHalf])
                console.log("zoneModels[PitchZones.rightHalf]", zoneModels[PitchZones.rightHalf])
                console.log("zoneModels[PitchZones.center]", zoneModels[PitchZones.center])
            }
        }
    }

    QtObject {
        id: dragInfo
        property string name: ""
        property int sender: -1
        property int reciever: -1

        function clear() {
            name = ""
            sender = -1
            reciever = -1
        }
    }

    function registerDrop(playerName) {
        console.log("dropping", dragInfo.name, dragInfo.sender, dragInfo.reciever)
        if (dragInfo.reciever !== -1 && dragInfo.reciever !== dragInfo.sender)
        {
            var index = zoneModels[dragInfo.sender].indexOf(playerName)
            if (index !== -1)
                zoneModels[dragInfo.sender].splice(index, 1)

            zoneModels[dragInfo.reciever].push(playerName)
        }

        dragInfo.clear()
    }

    function getPlayerZone(name) {
        var zoneIndex
        zoneModels.forEach(function(element) {
            if (element.indexOf(name) !== -1)
            {
                zoneIndex = zoneModels.indexOf(element)
            }
        })
        return zoneIndex
    }

    Repeater {
        id: playerHandles
        model: allPlayersModel

        delegate: PlayerHandle {
            player: allPlayersModel.getPlayer(index)
            x: {
                console.log(player.name, getPlayerZone(player.name))
                scheme.benchPlayerZone(index).x
            }
            y: scheme.benchPlayerZone(index).y

            onDragActiveChanged: {
                if (dragActive) {
                    Drag.start()
                    dragInfo.name = player.name
                }
                else
                {
                    Drag.drop()
                    registerDrop(player.name)
                }
            }
        }
    }
}
