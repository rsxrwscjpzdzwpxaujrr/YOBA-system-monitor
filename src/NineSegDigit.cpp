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

#include "NineSegDigit.hpp"

#include <QEasingCurve>
#include <QPen>
#include <QTransform>
#include <QObject>

#include "Segment.hpp"

NineSegDigit::NineSegDigit(QQuickItem* parent) :
            QQuickPaintedItem(parent),
            digit(' '),
            color(0.0f, 0.0f, 0.0f, 0),
            offColor(0.0f, 0.0f, 0.0f, 0),
            segmentLength(0.0f),
            segmentWidth(0.0f),
            off(0.0f) {
    setAntialiasing(true);

    for (int i = 0; i < 9; i++) {
        segments[i] = new Segment(offColor, 600, QEasingCurve::OutExpo, this);

        QObject::connect(segments[i], &Segment::colorChanged, this, [this] {
            update();
        });
    }
}

NineSegDigit::~NineSegDigit() {
    for (int i = 0; i < 9; i++) {
        delete segments[i];
    }
}

void
NineSegDigit::paint(QPainter* painter) {
    painter->setPen(Qt::PenStyle::NoPen);

    float shift = 0.25f;

    painter->setBrush(point ? color : offColor);

    painter->drawEllipse({
             segmentLength + (segmentWidth * 1.2f) + pointSize * 0.75f,
             segmentLength * 2.0f + segmentWidth - pointSize * 0.5f
         },
         pointSize / 2.0f,
         pointSize / 2.0f
    );

    painter->setTransform(
        QTransform(
            1.0f,   0.0f,
            -shift, 1.0f,
            1.0f,   1.0f
        ),
        true
    );

    painter->translate(segmentLength * (shift * 2.0f), 0.0f);

    for (int i = 0; i < 9; i++) {
        painter->setBrush(segments[i]->color());
        painter->drawPolygon(segments[i]->pol());
    }
}

void
NineSegDigit::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry.height() == oldGeometry.height())
        return;

    segmentLength = (newGeometry.height() - segmentWidth) / 2.0f;

    setWidth(segmentLength * 1.8f);

    initPols();
}

QPolygonF
NineSegDigit::verticalSegment() {
    QPolygonF pol;

    float width = segmentWidth;
    float length = segmentLength;

    float halfWidth = width * 0.5f;

    pol.append({ halfWidth, halfWidth + off                      });
    pol.append({     width, halfWidth + off + halfWidth          });
    pol.append({     width, halfWidth - off - halfWidth + length });
    pol.append({ halfWidth, halfWidth - off             + length });
    pol.append({ 0.0f     , halfWidth - off - halfWidth + length });
    pol.append({ 0.0f     , halfWidth + off + halfWidth          });

    return pol;
}

QPolygonF
NineSegDigit::horizontalSegment() {
    QPolygonF pol;

    float width = segmentWidth;
    float length = segmentLength;

    float halfWidth = width * 0.5f;

    pol.append({ halfWidth + off                     , halfWidth });
    pol.append({ halfWidth + off + halfWidth         ,     width });
    pol.append({ halfWidth - off - halfWidth + length,     width });
    pol.append({ halfWidth - off             + length, halfWidth });
    pol.append({ halfWidth - off - halfWidth + length, 0.0f      });
    pol.append({ halfWidth + off + halfWidth         , 0.0f      });

    return pol;
}

QPolygonF
NineSegDigit::weirdSegment() {
    QPolygonF pol;

    float width = segmentWidth;
    float length = segmentLength;

    float halfLength = length * 0.5f;

    pol.append({ halfLength       + width / 5.0f + width, -off         + length });
    pol.append({ halfLength       + width / 5.0f        , -off         + length });
    pol.append({     length - off - width * 0.55f       ,  off + width          });
    pol.append({     length - (off * 1.3f)              ,  off + width * 1.1f   });
    pol.append({     length - off                       ,  off + width + width  });

    return pol;
}

void
NineSegDigit::initPols() {
    if (segmentLength == 0.0f || segmentWidth == 0.0f) {
        return;
    }

    QPolygonF horPol = horizontalSegment();
    QPolygonF verPol = verticalSegment();

    QPolygonF pols[9];

    pols[0] = horPol;
    pols[3] = horPol;
    pols[6] = horPol;

    pols[1] = verPol;
    pols[2] = verPol;
    pols[4] = verPol;
    pols[5] = verPol;

    pols[1].translate(segmentLength, 0.0f);
    pols[2].translate(segmentLength, segmentLength);
    pols[3].translate(0.0,           segmentLength * 2.0f);
    pols[4].translate(0.0,           segmentLength);
    pols[6].translate(0.0,           segmentLength);

    pols[7] = weirdSegment();

    for (QPointF p: pols[7]) {
        pols[8].append(QPointF(segmentLength - p.x(), segmentLength - p.y()));
    }

    pols[8].translate(segmentWidth, segmentLength + segmentWidth);

    for (int i = 0; i < 9; i++) {
        segments[i]->setPol(pols[i]);
    }
}

void
NineSegDigit::setDigit(QChar digit) {
    NineSegDigit::digit = digit;

    switch (digit.toLatin1()) {
    case '0':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(color);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(offColor);
        segments[7]->setEndColor(color);
        segments[8]->setEndColor(color);
        break;

    case '1':
        segments[0]->setEndColor(offColor);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(offColor);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(offColor);
        segments[7]->setEndColor(color);
        segments[8]->setEndColor(offColor);
        break;

    case '2':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(offColor);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(color);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '3':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(color);
        segments[8]->setEndColor(offColor);
        break;

    case '4':
        segments[0]->setEndColor(offColor);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(offColor);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '5':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '6':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(color);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '7':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(offColor);
        segments[3]->setEndColor(offColor);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(offColor);
        segments[7]->setEndColor(color);
        segments[8]->setEndColor(color);
        break;

    case '8':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(color);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '9':
        segments[0]->setEndColor(color);
        segments[1]->setEndColor(color);
        segments[2]->setEndColor(color);
        segments[3]->setEndColor(color);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(color);
        segments[6]->setEndColor(color);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;

    case '/':
        segments[0]->setEndColor(offColor);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(offColor);
        segments[3]->setEndColor(offColor);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(offColor);
        segments[7]->setEndColor(color);
        segments[8]->setEndColor(color);
        break;

    default:
        segments[0]->setEndColor(offColor);
        segments[1]->setEndColor(offColor);
        segments[2]->setEndColor(offColor);
        segments[3]->setEndColor(offColor);
        segments[4]->setEndColor(offColor);
        segments[5]->setEndColor(offColor);
        segments[6]->setEndColor(offColor);
        segments[7]->setEndColor(offColor);
        segments[8]->setEndColor(offColor);
        break;
    }
}

void
NineSegDigit::setColor(const QColor& color) {
    NineSegDigit::color = color;

    setDigit(digit);
}

void
NineSegDigit::setOffColor(const QColor& offColor) {
    NineSegDigit::offColor = offColor;

    setDigit(digit);
}

void
NineSegDigit::setPoint(bool point) {
    NineSegDigit::point = point;
}

void
NineSegDigit::setSegmentWidth(float segmentWidth) {
    if (NineSegDigit::segmentWidth == segmentWidth)
        return;

    NineSegDigit::segmentWidth = segmentWidth;

    off = segmentWidth / 6.0f;
    pointSize = segmentWidth * 1.333f;

    initPols();
}
