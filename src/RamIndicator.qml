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

    property real value: Backend.getUsedRam() / Backend.totalRam;

    width: 400
    height: 400

    Timer {
        id: timer

        interval: 1000
        repeat: true
        running: true

        onTriggered: {
            value = Backend.getUsedRam() / Backend.totalRam;
            anim.start();
        }
    }

    DialGauge {
        id: indicator

        anchors.horizontalCenter: parent.horizontalCenter

        PropertyAnimation on value {
            id: anim

            easing.type: Easing.InOutQuad
            duration: 950
            to: root.value
        }
    }

    function getText(value) {
        let totalRam = Backend.totalRam / (1024 * 1024 * 1024);
        let usedRam = Backend.getUsedRam() / (1024 * 1024 * 1024);
        let text;

        if (value > 0.9)
            text = `RAM: **${usedRam.toFixed(1)}** / ${totalRam.toFixed(1)} GiB`;
        else
            text = `RAM: ${usedRam.toFixed(1)} / ${totalRam.toFixed(1)} GiB`;

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
    }

    DropShadow {
        anchors.fill: text

        horizontalOffset: 0
        verticalOffset: 12
        radius: 12.0
        samples: 6
        color: "#80000000"
        source: text
    }
}
