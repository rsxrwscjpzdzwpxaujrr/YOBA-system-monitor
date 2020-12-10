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

#pragma once

#include <QtQml>
#include <QQuickPaintedItem>
#include <QPolygonF>
#include <QColor>
#include <QEasingCurve>

class Segment : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor endColor READ endColor WRITE setEndColor NOTIFY endColorChanged)

public:
    Segment(
        const QColor& color,
        int animDuration,
        const QEasingCurve& easingCurve,
        QObject* parent=nullptr
    );

    QColor color() const;
    QPolygonF pol() const;
    QColor endColor() const;

public slots:
    void setColor(const QColor& color);
    void setEndColor(const QColor& endColor);
    void setPol(const QPolygonF& polygon);

signals:
    void colorChanged();
    void onChanged();
    void endColorChanged();

private:
    QPropertyAnimation animation;
    QColor m_color;
    QColor m_endColor;
    QPolygonF m_pol;
    bool m_on;
};
