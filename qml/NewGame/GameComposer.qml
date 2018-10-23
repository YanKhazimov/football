import QtQuick 2.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root

    anchors.fill: parent

    property var allPlayersModel: null

    property var benchModel: []
    property var homeModel: []
    property var awayModel: []
    property var centerModel: []

    onBenchModelChanged: console.log(benchModel)

    onAllPlayersModelChanged: {
        var bm = []
        for (var i = 0; i < allPlayersModel.length; ++i)
        {
            if (allPlayersModel.getPlayer(i) !== null)
                bm.push(allPlayersModel.getPlayer(i).name)
        }
        benchModel = bm
        homeModel = []
        awayModel = []
        centerModel = []
    }

    PitchScheme {
        id: scheme
        benchLength: allPlayersModel.length

        onDroppedBench: {
            homeModel.push(dragSource.player.name)
            benchModel.splice(benchModel.indexOf(dragSource.player.name), 1)
            awayModel.splice(awayModel.indexOf(dragSource.player.name), 1)
            centerModel.splice(centerModel.indexOf(dragSource.player.name), 1)
        }

        onDroppedLeft: {
            homeModel.push(dragSource.player.name)
            benchModel.splice(benchModel.indexOf(dragSource.player.name), 1)
            awayModel.splice(awayModel.indexOf(dragSource.player.name), 1)
            centerModel.splice(centerModel.indexOf(dragSource.player.name), 1)
        }

        onDroppedRight: {
            awayModel.push(dragSource.player.name)
            benchModel.splice(benchModel.indexOf(dragSource.player.name), 1)
            homeModel.splice(awayModel.indexOf(dragSource.player.name), 1)
            centerModel.splice(centerModel.indexOf(dragSource.player.name), 1)
        }

        onDroppedCenter: {
            centerModel.push(dragSource.player.name)
            benchModel.splice(benchModel.indexOf(dragSource.player.name), 1)
            homeModel.splice(awayModel.indexOf(dragSource.player.name), 1)
            awayModel.splice(awayModel.indexOf(dragSource.player.name), 1)
        }
    }

    Image {
        x: scheme.pitchCenterCoords.x
        y: scheme.pitchCenterCoords.y
        width: Sizes.elementButtonSize.width
        height: Sizes.elementButtonSize.height
        source: "qrc:/img/play.png"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                console.log("benchModel", benchModel)
                console.log("homeModel", homeModel)
                console.log("awayModel", awayModel)
                console.log("centerModel", centerModel)
            }
        }
    }

    Repeater {
        id: benchPlayers
        model: allPlayersModel

        delegate: PlayerHandle {
            x: scheme.benchPlayerZone(index).x
            y: scheme.benchPlayerZone(index).y
            player: allPlayersModel.getPlayer(index)
        }
    }

    Repeater {
        id: homePlayers
        model: null

        delegate: PlayerHandle {
            x: 0
            y: scheme.benchHeight + index * Sizes.playerHandleSize
            player: allPlayersModel.getPlayer(index)
        }
    }

    Repeater {
        id: awayPlayers
        model: null

        delegate: PlayerHandle {
            x: scheme.width - Sizes.playerHandleSize
            y: scheme.benchHeight + index * Sizes.playerHandleSize
            player: allPlayersModel.getPlayer(index)
        }
    }


}
