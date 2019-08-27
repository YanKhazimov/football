import QtQuick 2.12
import QtQuick.Layouts 1.3
import "qrc:/qml/visualStyles"

Rectangle {
    id: root
    property var theme: null
    property var points: null
    readonly property int offset: Sizes.margin
    readonly property int __value: 0
    readonly property int __idx: 1

    height: 100
    color: theme.primaryColor

    function updatePoints(ratingValues) {
        var res = []
        ratingValues.forEach(function(index){
            res.push(Qt.point(res.length, index))
        })
        points = res

        var ctx = canvas.getContext("2d");
        if (ctx !== null)
        {
            ctx.reset();
            canvas.requestPaint()
        }
    }

    function findBest(betterThan) {
        var value = root.points.length === 0 ? "" : root.points[0].y
        var index = root.points.length === 0 ? -1 : 0
        for(var i = 1; i < root.points.length; i++)
        {
            if (betterThan(root.points[i].y, value))
            {
                value = root.points[i].y
                index = i
            }
        }
        var result = []
        result[__value] = value
        result[__idx] = index
        return result
    }

    function max() {
        return findBest(function(a, b) {return a > b})
    }

    function min() {
        return findBest(function(a, b) {return a < b})
    }

    Canvas {
      id: canvas
      anchors {
          fill: root
          margins: root.offset
      }
      readonly property int r: 2

      function mapY (y, minY, maxY) {
          if (minY === maxY)
              return height / 2
          var res = height - height * (y - minY) / (maxY - minY)
          return Math.min(height - r - 1, Math.max(r + 1, res))
      }

      function mapX (i, count) {
          var res
          if (count === 1)
              res = width / 2
          else
              res = width / (count - 1) * i

          return Math.min(width - r - 1, Math.max(r + 1, res))
      }

      onPaint: {
          var ctx = canvas.getContext('2d');

          ctx.save()
          ctx.lineWidth = 2;
          ctx.beginPath();

          var maxY = root.max()
          var minY = root.min()

          ctx.strokeStyle = root.theme.secondaryColor;
          var startX = mapX(0, root.points.length)
          var startY = mapY(root.points[0].y, minY[__value], maxY[__value])
          ctx.moveTo(startX, startY)

          for(var i = 1; i < root.points.length; i++)
          {
              var x = mapX(i, root.points.length)
              var y = mapY(root.points[i].y, minY[__value], maxY[__value])
              ctx.lineTo(x, y)
          }
          ctx.stroke();
      }
    }

    Rectangle {
        anchors {
            bottom: canvas.top
            bottomMargin: {
                if (minMark.text !== maxMark.text)
                    return 0
                return -canvas.height/2 + canvas.r
            }
            left: canvas.left
            leftMargin: canvas.mapX(max()[__idx], root.points.length) - width / 2
        }
        width: maxMark.width
        height: maxMark.height
        color: root.theme.primaryColor

        Text {
            id: maxMark
            text: max()[__value]
            color: root.theme.secondaryColor
            anchors.centerIn: parent
        }
    }

    Rectangle {
        anchors {
            top: canvas.bottom
            left: canvas.left
            leftMargin: canvas.mapX(min()[__idx], root.points.length) - width / 2
        }
        width: minMark.width
        height: minMark.height
        color: root.theme.primaryColor

        Text {
            id: minMark
            text: min()[__value]
            color: root.theme.secondaryColor
            visible: minMark.text !== maxMark.text
            anchors.centerIn: parent
        }
    }
}
