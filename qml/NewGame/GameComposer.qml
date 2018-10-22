import QtQuick 2.0
import QtGraphicalEffects 1.0
import "qrc:/qml/visualStyles"

Rectangle {
    id: root

    anchors.fill: parent

    PitchScheme {
        id: scheme
        benchModel: globalStatsModel
    }

    Repeater {
        model: globalStatsModel

        delegate: PlayerHandle {
            x: scheme.benchPlayerZone(index).x
            y: scheme.benchPlayerZone(index).y

            photoSource: globalStatsModel.getPlayer(model.PlayerName).photo
            topText: model.PlayerName
        }
    }

}
