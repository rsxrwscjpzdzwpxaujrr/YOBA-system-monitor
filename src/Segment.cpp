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

#include "Segment.hpp"

Segment::Segment(
            const QColor& color,
            int animDuration,
            const QEasingCurve& easingCurve,
            QObject* parent
        ) : QObject(parent), m_color(color), m_endColor(color)  {
    animation.setTargetObject(this);
    animation.setPropertyName("color");
    animation.setDuration(animDuration);
    animation.setEasingCurve(easingCurve);
}

QColor
Segment::color() const {
    return m_color;
}

void
Segment::setColor(const QColor& color) {
    m_color = color;

    emit colorChanged();
}

void
Segment::setEndColor(const QColor& endColor) {
    if (m_endColor == endColor)
        return;

    m_endColor = endColor;
    emit endColorChanged();

    if (animation.duration() == 0) {
        setColor(endColor);
        return;
    }

    if (animation.state() == QPropertyAnimation::State::Running) {
        int time = animation.currentTime();
        animation.stop();
        animation.setCurrentTime(time);
    }

    animation.setEndValue(endColor);
    animation.start();
}

void
Segment::setPol(const QPolygonF& polygon) {
    m_pol = polygon;
}

QPolygonF
Segment::pol() const {
    return m_pol;
}

QColor
Segment::endColor() const {
    return m_endColor;
}
