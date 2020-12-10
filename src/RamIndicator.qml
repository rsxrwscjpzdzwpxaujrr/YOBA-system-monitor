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

import Local 1.0

Item {
    id: root

    width: 400
    height: 400

    property real value: Backend.getUsedRam() / Backend.totalRam

    Timer {
        id: timer

        interval: 300
        repeat: true
        running: true

        onTriggered: {
            value = Backend.getUsedRam() / Backend.totalRam
        }
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
        let totalRam = Backend.totalRam / (1024 * 1024 * 1024);
        let usedRam = Backend.getUsedRam() / (1024 * 1024 * 1024);

        totalRam = Math.round(totalRam * 10).toString();
        usedRam = Math.round(usedRam * 10).toString();

        while (totalRam.length < 3) {
            totalRam = " " + totalRam;
        }

        while (usedRam.length < 3) {
            usedRam = " " + usedRam;
        }

        return `${usedRam}/${totalRam}`;
    }

    Rectangle {
        id: display

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: indicator.bottom
        anchors.topMargin: 30

        color: "#002b36"

        width: inDisplay.width + 30
        height: inDisplay.height + 30

        NineSegDisp {
            id: inDisplay

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            height: 40
            segmentWidth: 5

            color: root.value > 0.8 ? "#fa4441" : "#3d9adb"
            offColor: "#18" + color.toString().substring(7, 1)

            value: getText(root.value)

            digits: 7

            point1: 1
            point2: 5

            layer.enabled: true
            layer.effect: Glow {
                radius: 15
                samples: 13
                color: inDisplay.color
                spread: 0
            }
        }

        layer.enabled: true
        layer.effect: InnerShadow {
            horizontalOffset: 0
            verticalOffset: 20
            radius: 40.0
            samples: 12
            color: "#30000000"
        }
    }

    Text {
        id: text

        anchors.verticalCenter: display.verticalCenter
        anchors.right: display.left
        anchors.rightMargin: 10

        text: "RAM: "

        color: "#93a1a1"

        font.family: "mono"
        font.pointSize: 28
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


    Text {
        id: text2

        anchors.verticalCenter: display.verticalCenter
        anchors.left: display.right
        anchors.leftMargin: 15

        text: " GiB"

        color: "#93a1a1"

        font.family: "mono"
        font.pointSize: 28
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
