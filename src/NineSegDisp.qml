/*
 * Copyright (c) 2020, 2022 Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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
import QtGraphicalEffects 1.0
import Local 1.0

ListView {
    id: root

    orientation: ListView.Horizontal

    property string value: "   "
    property int digits: 1
    property int point1: -1
    property int point2: -1
    property color color: "black"
    property color offColor: "white"
    property real segmentWidth: 5.5

    model: ListModel {
        id: model
    }

    function update() {
        let value = root.value;

        while (value.length < digits) {
            value = " " + value;
        }

        for (let i = 0; i < digits; i++) {
            model.set(i, { "digit": value[i], "point": i === point1 | i === point2 });
        }
    }

    Component.onCompleted: update();
    onValueChanged: update()
    onPoint1Changed: update()
    onPoint2Changed: update()

    width: childrenRect.width

    spacing: 6
    interactive: false

    delegate: NineSegDigit {
        id: display

        digit: model.digit
        point: model.point

        segmentWidth: root.segmentWidth

        height: root.height

        color: root.color
        offColor: root.offColor

        nine: true

        MouseArea {
            anchors.fill: parent

            onClicked: display.nine = !display.nine
        }
    }
}
