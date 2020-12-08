/*
 * Copyright (c) 2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtGraphicalEffects 1.12

Canvas {
    id: root

    property int size: 400
    property real value: 0.0

    width: size
    height: size * 0.88

    property real markLength: 30
    property real markOffset: 10

    property real startAngle: 45 * (Math.PI / 180)
    property real endAngle: 315 * (Math.PI / 180)

    clip: true

    Rectangle {
        id: background

        z: -2

        width: size
        height: size

        radius: size / 2
        color: "#002b36"
    }

    InnerShadow {
        z: -1

        anchors.fill: background

        horizontalOffset: 0
        verticalOffset: 20
        radius: 40.0
        samples: 12
        color: "#30000000"
        source: background
    }

    function drawMarks(ctx) {
        ctx.shadowBlur = 10;

        let strokeStart = markOffset;
        let strokeEnd = strokeStart + markLength
        let step = (endAngle - startAngle) / 20;

        for (let i = startAngle, line = 0; i <= endAngle; i += step, line++) {
            let halfSize = size / 2;

            if (line <= 11) {
                ctx.strokeStyle = "#859900";
            } else if (line > 17) {
                ctx.strokeStyle = "#dc322f";
            } else {
                ctx.strokeStyle = "#cb4b16";
            }

            if (line % 2 == 0) {
                ctx.shadowColor = ctx.strokeStyle;
                ctx.lineWidth = 12

                strokeEnd = strokeStart + markLength
            } else {
                ctx.shadowColor = "#00000000";
                ctx.lineWidth = 6

                ctx.strokeStyle = "#60" + ctx.strokeStyle.substring(7, 1);

                strokeEnd = strokeStart + markLength / 2
            }

            ctx.beginPath();
            ctx.moveTo(
                (-Math.sin(i) * (halfSize - strokeStart)),
                (Math.cos(i) * (halfSize - strokeStart))
            );

            ctx.lineTo(
                (-Math.sin(i) * (halfSize - strokeEnd)),
                (Math.cos(i) * (halfSize - strokeEnd))
            );

            ctx.stroke();
        }

        ctx.shadowColor = "#00000000";
    }

    function drawPointer(ctx) {
        let halfSize = size / 2;
        let length = halfSize * 0.9;
        let width = 20;
        let angle = startAngle + (value * (endAngle - startAngle)) - Math.PI

        ctx.save();
        ctx.rotate(angle);

        ctx.beginPath();

        ctx.moveTo(0, width + 10);
        ctx.lineTo(width, 10);
        ctx.lineTo(0, -length);
        ctx.lineTo(-width, 10);

        ctx.closePath();

        ctx.fillStyle = "#d33682"
        ctx.fill();

        ctx.restore();
    }

    onPaint: {
        var ctx = getContext("2d");

        ctx.clearRect(0, 0, width, height);
        ctx.translate(size / 2, size / 2);
        ctx.lineWidth = 12

        drawMarks(ctx);

        ctx.translate(-size / 2, -size / 2);
    }

    Canvas {
        id: pointer

        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");

            ctx.clearRect(0, 0, width, height);
            ctx.translate(size / 2, size / 2);
            ctx.lineWidth = 12

            drawPointer(ctx);

            ctx.translate(-size / 2, -size / 2);
        }
    }

    onValueChanged: {
        pointer.requestPaint();
    }

    DropShadow {
        anchors.fill: pointer

        horizontalOffset: 0
        verticalOffset: 6
        radius: 12.0
        samples: 6
        color: "#40000000"
        source: pointer
    }
}
