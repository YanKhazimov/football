import QtQuick 2.0
import QtGraphicalEffects 1.0
import "."
import ".."
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
            var player = allPlayersModel.getPlayer(i)
            if (player !== null)
                startingModel.push(player.name)
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
                var offset = Qt.point(0, -Sizes.playerHandleRatingHeight/2)
                showHint(zone, zoneModels[zone].length, zoneModels[zone].length + 1, offset)

                updateTotals(zone, allPlayersModel.getPlayer(dragInfo.name).getRating())
            }
        }

        onDragExit: {
            var idx = zoneModels[zone].indexOf(dragInfo.name)
            if (idx > -1)
                zoneModels[zone].splice(idx, 1)

            dragInfo.reciever = -1

            hideHint()
            adjustFormation(zone, zoneModels[zone].length)

            updateTotals(zone, 0)
        }
    }

    function split() {
        teamSplitter.split(zoneModels[PitchZones.leftHalf],
                           zoneModels[PitchZones.rightHalf],
                           zoneModels[PitchZones.center])

        console.log("teamSplitter.homeRoster", teamSplitter.homeRoster)
        console.log("teamSplitter.awayRoster", teamSplitter.awayRoster)
    }

    ColoredImage {
        id: splitButton
        width: 4 * Sizes.elementButtonSize.width
        height: 4 * Sizes.elementButtonSize.height
        x: scheme.pitchCenterCoords.x - width/2
        y: scheme.pitchCenterCoords.y - height/2
        source: "qrc:/img/balance.png"
        color: mouseArea.containsMouse ? theme.secondaryFillColor : "white"
        Behavior on rotation { PropertyAnimation { duration: 500 } }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
//                console.log("zoneModels[PitchZones.bench]", zoneModels[PitchZones.bench])
//                console.log("zoneModels[PitchZones.leftHalf]", zoneModels[PitchZones.leftHalf])
//                console.log("zoneModels[PitchZones.rightHalf]", zoneModels[PitchZones.rightHalf])
//                console.log("zoneModels[PitchZones.center]", zoneModels[PitchZones.center])

                parent.rotation += 720 - parent.rotation % 720
                split()
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
        if (dragInfo.reciever === -1)
            dragInfo.reciever = dragInfo.sender

        console.log("dropping", dragInfo.name, dragInfo.sender, dragInfo.reciever)

        var index = zoneModels[dragInfo.reciever].indexOf(playerName)
        if (index === -1)
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
                    updateTotals(dragInfo.reciever, 0)
                    adjustFormation(dragInfo.reciever, zoneModels[dragInfo.reciever].length)
                    scheme.hideHint()

                    dragInfo.clear()
                }
            }
        }
    }

    function updateTotals(zone, offset) {
        if (zone === PitchZones.leftHalf)
        {
            scheme.homeTotal = offset
            zoneModels[PitchZones.leftHalf].forEach(function(element){
                scheme.homeTotal += allPlayersModel.getPlayer(element).getRating()
            })
        }
        else if (zone === PitchZones.rightHalf)
        {
            scheme.awayTotal = offset
            zoneModels[PitchZones.rightHalf].forEach(function(element){
                scheme.awayTotal += allPlayersModel.getPlayer(element).getRating()
            })
        }
    }
}
