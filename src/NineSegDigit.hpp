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
#include <QPainter>
#include <QColor>

class Segment;

class NineSegDigit : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(float  segmentWidth MEMBER segmentWidth WRITE setSegmentWidth)
    Q_PROPERTY(QChar  digit        MEMBER digit        WRITE setDigit)
    Q_PROPERTY(bool   point        MEMBER point        WRITE setPoint)
    Q_PROPERTY(QColor color        MEMBER color        WRITE setColor)
    Q_PROPERTY(QColor offColor     MEMBER offColor     WRITE setOffColor)
    QML_ELEMENT

public:
    NineSegDigit(QQuickItem* parent=nullptr);
    ~NineSegDigit();

    void paint(QPainter* painter) override;

protected:
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

public slots:
    void setDigit(QChar digit);
    void setColor(const QColor& color);
    void setOffColor(const QColor& offColor);
    void setPoint(bool point);
    void setSegmentWidth(float segmentWidth);

private:
    Segment* segments[9];

    QChar digit;
    bool point;

    QColor color;
    QColor offColor;

    float segmentLength;
    float segmentWidth;
    float pointSize;
    float off;

    QPolygonF verticalSegment();
    QPolygonF horizontalSegment();
    QPolygonF weirdSegment();

    void initPols();
};
