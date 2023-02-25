/*
 * Copyright (c) 2020-2023, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

import QtQuick
import QtQuick.Window
import Local

Window {
    id: root

    width: 1000
    height: 550

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    visible: true
    title: "YOBA System Monitor"

    color: "#073642"

    CpuIndicator {
        id: cpuIndicator

        anchors.left: parent.left
        anchors.top: parent.top

        anchors.margins: 60

        ScaleAnimator on scale {
            from: 0.5
            to: 1

            duration: 800
            running: true

            easing.type: Easing.OutCubic
        }

        OpacityAnimator on opacity {
            from: 0
            to: 1

            duration: 1000
            running: true

            easing.type: Easing.OutCubic
        }
    }

    RamIndicator {
        id: ramIndicator

        anchors.top: parent.top
        anchors.right: parent.right

        anchors.margins: 60

        ScaleAnimator on scale {
            from: 0.5
            to: 1

            duration: 800
            running: true

            easing.type: Easing.OutCubic
        }

        OpacityAnimator on opacity {
            from: 0
            to: 1

            duration: 1000
            running: true

            easing.type: Easing.OutCubic
        }
    }
}
