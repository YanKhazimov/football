import QtQuick 2.0
import QtGraphicalEffects 1.0
import "."
import ".."
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    anchors.fill: parent

    property var theme: null
    property var allPlayersModel: null
    property var zoneModels: []
    property real benchScrollOffset: 0
    property int splitIndex: 0

    Component.onCompleted: showControlPanel()

    function resetSplits() {
        teamSplitter.splitVariants = []
        splitIndex = 0
        controlPanel.splitText = lang.splitVerb
    }

    function reloadModel() {
        var startingModel = []
        for (var i = 0; i < allPlayersModel.rowCount(); ++i)
        {
            var player = allPlayersModel.getPlayer(i)
            if (player !== null)
                startingModel.push(player.name)
        }
        zoneModels = [ startingModel, [], [], [] ]

        benchScrollOffset = 0

        adjustFormation(PitchZones.bench)
        adjustFormation(PitchZones.leftHalf)
        adjustFormation(PitchZones.rightHalf)
        adjustFormation(PitchZones.center)

        updateTotals()

        resetSplits()
    }

    onAllPlayersModelChanged: reloadModel()

    Connections {
        target: allPlayersModel
        onModelReset: reloadModel()
    }

    function benchAll() {
        zoneModels = [zoneModels[PitchZones.bench].concat(zoneModels[PitchZones.leftHalf],
                      zoneModels[PitchZones.rightHalf], zoneModels[PitchZones.center]),
                      [], [], []]

        adjustFormation(PitchZones.bench)
        adjustFormation(PitchZones.leftHalf)
        adjustFormation(PitchZones.rightHalf)
        adjustFormation(PitchZones.center)

        resetSplits()
    }

    function adjustFormation(zone, newSize) {
        if (newSize === undefined)
            newSize = zoneModels[zone].length

        for (var i = 0; i < playerHandles.count; ++i)
        {
            var name = playerHandles.itemAt(i).player.name
            var indexInZone = zoneModels[zone].indexOf(name)
            if (indexInZone !== -1)
            {
                var dropPosition = scheme.calculatePosition(zone, indexInZone, newSize)

                if (zone === PitchZones.bench)
                {
                    dropPosition.x += benchScrollOffset * scheme.getBenchPlayerSpace()
                    var benchPosition = indexInZone + benchScrollOffset
                    playerHandles.itemAt(i).visible = (benchPosition >= 0 && benchPosition <= scheme.getBenchCapacity() - 1)
                }

                playerHandles.itemAt(i).x = dropPosition.x - playerHandles.itemAt(i).width/2
                playerHandles.itemAt(i).y = dropPosition.y - playerHandles.itemAt(i).height/2
            }
        }
    }

    PitchScheme {
        id: scheme
        benchLength: allPlayersModel.rowCount()
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

                var extraHomeDiff
                if (zone == PitchZones.leftHalf)
                    extraHomeDiff = allPlayersModel.getPlayerRating(dragInfo.name)
                else if (zone == PitchZones.rightHalf)
                    extraHomeDiff = -allPlayersModel.getPlayerRating(dragInfo.name)

                updateTotals(extraHomeDiff)
            }
        }

        onDragExit: {
            var idx = zoneModels[zone].indexOf(dragInfo.name)
            if (idx > -1)
                zoneModels[zone].splice(idx, 1)

            dragInfo.reciever = -1

            hideHint()
            adjustFormation(zone, zoneModels[zone].length)

            updateTotals()
        }

        onBenchScrolled: {
            if (offset > 0 && benchScrollOffset + offset <= 0 ||
                    offset < 0 && benchScrollOffset + offset + zoneModels[PitchZones.bench].length >= getBenchCapacity())
            {
                benchScrollOffset += offset
                adjustFormation(PitchZones.bench)
            }
        }
    }

    function split() {
        var splitVariant
        if (teamSplitter.splitVariants.length === 0)
        {
            teamSplitter.split(zoneModels[PitchZones.leftHalf],
                           zoneModels[PitchZones.rightHalf],
                           zoneModels[PitchZones.center],
                           5)

            splitVariant = teamSplitter.splitVariants[0]
        }
        else
        {
            splitIndex = (splitIndex + 1) % teamSplitter.splitVariants.length
            splitVariant = teamSplitter.splitVariants[splitIndex]
        }

        splitVariantIndicators.selectedIndex = splitIndex

        zoneModels[PitchZones.leftHalf] = splitVariant.splice(0, splitVariant.length/2)
        zoneModels[PitchZones.rightHalf] = splitVariant.splice(0, splitVariant.length) //???
        zoneModels[PitchZones.center] = []

        adjustFormation(PitchZones.leftHalf)
        adjustFormation(PitchZones.rightHalf)
        adjustFormation(PitchZones.center)

        updateTotals()

        controlPanel.splitText = (splitVariant.length === 0) ?
                    lang.splitVerb :
                    "%1 %2/%3".arg(lang.splitNoun).arg(splitIndex + 1).arg(teamSplitter.splitVariants.length)
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
        console.debug("dropping", dragInfo.name, dragInfo.sender, dragInfo.reciever)

        if (dragInfo.reciever === -1)
            dragInfo.reciever = dragInfo.sender
        var index = zoneModels[dragInfo.reciever].indexOf(playerName)
        if (index === -1)
            zoneModels[dragInfo.reciever].push(playerName)

        if (dragInfo.reciever !== dragInfo.sender)
        {
            resetSplits()
        }
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
                    if (dragInfo.reciever == PitchZones.leftHalf || dragInfo.reciever == PitchZones.rightHalf)
                        updateTotals()
                    adjustFormation(dragInfo.reciever, zoneModels[dragInfo.reciever].length)
                    scheme.hideHint()

                    dragInfo.clear()
                }
            }
        }
    }

    function updateTotals(extraHomeDiff) {
        if (extraHomeDiff === undefined)
            extraHomeDiff = 0

        var homeDiff = extraHomeDiff

        zoneModels[PitchZones.leftHalf].forEach(function(element){
            homeDiff += allPlayersModel.getPlayerRating(element)
        })

        zoneModels[PitchZones.rightHalf].forEach(function(element){
            homeDiff -= allPlayersModel.getPlayerRating(element)
        })

        scheme.homeDiff = homeDiff
        scheme.awayDiff = -homeDiff
    }

    MouseArea {
        id: disabler
        visible: outcomeForm.visible
        enabled: visible
        anchors.fill: parent
        hoverEnabled: false
        propagateComposedEvents: false
        z: 1
    }

    OutcomeForm {
        id: outcomeForm
        anchors {
            bottom: scheme.bottom
            horizontalCenter: scheme.horizontalCenter
            margins: Sizes.featuredStats.smallMargin
        }
        width: 2 * scheme.pitchCenterRadius
        height: scheme.height - scheme.benchHeight - 2 * anchors.margins
        visible: false
        onGameAdded: benchAll()
        z: 2
    }

    Repeater {
        id: splitVariantIndicators
        model: teamSplitter.splitVariants.length
        property int selectedIndex: 0
        delegate: Rectangle {
            height: 2
            width: teamSplitter.splitVariants.length < 2 ? 0 : ((Sizes.elementButtonSize.width * 2  - 2 * (teamSplitter.splitVariants.length - 1)) / teamSplitter.splitVariants.length)
            color: model.index === splitVariantIndicators.selectedIndex ? theme.secondaryFillColor : theme.textColor
            x: controlPanel.x + Sizes.elementButtonSize.width / 2 + model.index * (width + 2)
            y: controlPanel.y + Sizes.elementButtonSize.height + Sizes.featuredStats.smallMargin + Sizes.elementButtonSize.height - 7
        }
    }

    TeamControlPanel {
        id: controlPanel
        z: -1
        anchors {
            right: root.left
            rightMargin: -Sizes.elementButtonSize.width
            bottom: root.bottom
        }

        theme: root.theme

        readonly property var callbacks: [benchAll, split, function(){
            outcomeForm.homeTeam = zoneModels[PitchZones.leftHalf]
            outcomeForm.awayTeam = zoneModels[PitchZones.rightHalf]
            outcomeForm.visible = true
        }]

        onButtonClicked: callbacks[idx]()

        PropertyAnimation {
            id: showAnimation
            duration: 300
            target: controlPanel
            property: "anchors.rightMargin"
            from: -Sizes.elementButtonSize.width
            to: Sizes.featuredStats.smallMargin
        }
    }

    function showControlPanel() {
        controlPanel.anchors.rightMargin = -Sizes.elementButtonSize.width
        showAnimation.start()
    }
}
