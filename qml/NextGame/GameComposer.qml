import QtQuick 2.0
import QtGraphicalEffects 1.0
import "."
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    anchors.fill: parent

    property QtObject theme: null
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
        theme: root.theme

        onDragEnter: {
            var sending = dragInfo.sender === -1
            if (sending)
            {
                dragInfo.sender = zone
            }

            dragInfo.reciever = zone

            if (sending === false)
            {
                adjustFormation(zone, zoneModels[zone].length + 1)
                var offset = Qt.point(0, -Sizes.fontPixelSize/2)
                showHint(zone, zoneModels[zone].length, zoneModels[zone].length + 1, offset)
            }
        }

        onDragExit: {
            var idx = zoneModels[zone].indexOf(dragInfo.name)
            if (idx > -1)
                zoneModels[zone].splice(idx, 1)

            if (dragInfo.reciever === zone) //???
                dragInfo.reciever = -1

            hideHint()
            adjustFormation(zone, zoneModels[zone].length)
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
        color: mouseArea.containsMouse ? theme.secondaryFillColor : "white"
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

        var index = zoneModels[dragInfo.sender].indexOf(playerName)
        if (index !== -1)
            zoneModels[dragInfo.sender].splice(index, 1)

        zoneModels[dragInfo.reciever].push(playerName)
    }

    Repeater {
        id: playerHandles
        model: allPlayersModel

        delegate: PlayerHandle {
            player: allPlayersModel.getPlayer(index)
            x: scheme.calculatePosition(PitchZones.bench, index, model.count).x - width/2
            y: scheme.calculatePosition(PitchZones.bench, index, model.count).y - height/2
            theme: root.theme

            onDragActiveChanged: {
                if (dragActive) {
                    Drag.start()
                    dragInfo.name = player.name
                }
                else
                {
                    Drag.drop()
                    registerDrop(player.name)

                    // loop
                    if (zoneModels[PitchZones.leftHalf].indexOf(player.name) !== -1)
                    {
                        adjustFormation(PitchZones.leftHalf, zoneModels[PitchZones.leftHalf].length)
                    }
                    if (zoneModels[PitchZones.rightHalf].indexOf(player.name) !== -1)
                    {
                        adjustFormation(PitchZones.rightHalf, zoneModels[PitchZones.rightHalf].length)
                    }
                    if (zoneModels[PitchZones.center].indexOf(player.name) !== -1)
                    {
                        adjustFormation(PitchZones.center, zoneModels[PitchZones.center].length)
                    }
                    if (zoneModels[PitchZones.bench].indexOf(player.name) !== -1)
                    {
                        adjustFormation(PitchZones.bench, zoneModels[PitchZones.bench].length)
                    }
                    scheme.hideHint()
                    dragInfo.clear()
                }
            }
        }
    }
}
