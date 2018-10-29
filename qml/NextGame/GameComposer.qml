import QtQuick 2.0
import QtGraphicalEffects 1.0
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

    function adjustFormation(zone, newSize) {
        for (var i = 0; i < playerHandles.count; ++i)
        {
            var name = playerHandles.itemAt(i).player.name
            var indexInZone = zoneModels[zone].indexOf(name)
            if (indexInZone !== -1)
            {
                var dropPosition = scheme.calculatePosition(zone, indexInZone, newSize)

                playerHandles.itemAt(i).x = dropPosition.x - playerHandles.itemAt(i).width/2
                playerHandles.itemAt(i).y = dropPosition.y - playerHandles.itemAt(i).height/2
            }
        }
    }

    PitchScheme {
        id: scheme
        benchLength: allPlayersModel.length

        onDragEnter: {
            if (dragInfo.sender === -1)
                dragInfo.sender = zone

            dragInfo.reciever = zone

            if (dragInfo.sender !== dragInfo.reciever)
            {
                showHint(zone, zoneModels[zone].length, zoneModels[zone].length + 1)

                if (zone === PitchZones.leftHalf)
                {
                    adjustFormation(zone, zoneModels[zone].length + 1)
                }
            }
        }

        onDragExit: {
            if (dragInfo.reciever === zone)
                dragInfo.reciever = -1

            hideHint()

            if (zone === PitchZones.leftHalf)
            {
                if (dragInfo.sender === zone)
                    adjustFormation(zone, zoneModels[zone].length - 1)
                else
                    adjustFormation(zone, zoneModels[zone].length)
            }
        }
    }

    Image {
        id: splitButton
        width: 2*Sizes.elementButtonSize.width
        height: 2*Sizes.elementButtonSize.height
        x: scheme.pitchCenterCoords.x - width/2
        y: scheme.pitchCenterCoords.y - height/2
        source: "qrc:/img/split.png"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                console.log("zoneModels[PitchZones.bench]", zoneModels[PitchZones.bench])
                console.log("zoneModels[PitchZones.leftHalf]", zoneModels[PitchZones.leftHalf])
                console.log("zoneModels[PitchZones.rightHalf]", zoneModels[PitchZones.rightHalf])
                console.log("zoneModels[PitchZones.center]", zoneModels[PitchZones.center])
            }
        }
    }

    ColorOverlay {
        source: splitButton
        anchors.fill: splitButton
        color: mouseArea.containsMouse ? "white" : "transparent"
        cached: true
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
    }

    Repeater {
        id: playerHandles
        model: allPlayersModel

        delegate: PlayerHandle {
            player: allPlayersModel.getPlayer(index)
            x: scheme.benchPlayerZone(index).x
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

                    var dropPosition

                    if (zoneModels[PitchZones.leftHalf].indexOf(player.name) !== -1)
                    {
                        dropPosition = scheme.calculatePosition(PitchZones.leftHalf,
                                                                zoneModels[dragInfo.reciever].length - 1,
                                                                zoneModels[dragInfo.reciever].length)
                        x = dropPosition.x - width/2
                        y = dropPosition.y - height/2
                    }
                    if (zoneModels[PitchZones.center].indexOf(player.name) !== -1)
                    {
                        dropPosition = scheme.calculatePosition(PitchZones.center,
                                                                zoneModels[dragInfo.reciever].length - 1,
                                                                zoneModels[dragInfo.reciever].length)
                        x = dropPosition.x - width/2
                        y = dropPosition.y - height/2
                    }

                    scheme.hideHint()
                    dragInfo.clear()
                }
            }
        }
    }
}
