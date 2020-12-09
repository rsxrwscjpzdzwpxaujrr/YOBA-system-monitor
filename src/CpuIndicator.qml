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
import QtGraphicalEffects 1.0

import Backend 1.0

Item {
    id: root

    width: 400
    height: 400

    property real value: 0

    Timer {
        id: timer

        interval: 300
        repeat: true
        running: true

        onTriggered: {
            value = Backend.getCpuLoad();
        }
    }

    Component.onCompleted: {
        value = Backend.getCpuLoad();
    }

    DialGauge {
        id: indicator

        anchors.horizontalCenter: parent.horizontalCenter

        value: root.value

        Behavior on value {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
                duration: timer.interval - 20
            }
        }
    }

    function getText(value) {
        let text;

        if (value > 0.9)
            text = `CPU: **${(value * 100).toFixed(1)}%**`;
        else
            text = `CPU: ${(value * 100).toFixed(1)}%`;

        return text;
    }

    Text {
        id: text

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: indicator.bottom
        anchors.topMargin: 30

        text: getText(value);

        color: "#93a1a1"

        font.family: "mono"
        font.pointSize: 26
        textFormat: Text.MarkdownText

        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 12
            radius: 12.0
            samples: 6
            color: "#80000000"
        }
    }
}
