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

#include "NineSegDigit.hpp"

#include <QEasingCurve>
#include <QObject>
#include <QtMath>
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QMatrix4x4>

#include "Segment.hpp"

NineSegDigit::NineSegDigit(QQuickItem* parent) :
            QQuickItem(parent),
            geometryValid(false),
            shiftValid(false),
            digit(" "),
            point(false),
            nine(false),
            color(0.0f, 0.0f, 0.0f, 0),
            offColor(0.0f, 0.0f, 0.0f, 0),
            segmentLength(-1.0f),
            segmentWidth(-1.0f),
            off(-1.0f),
            shift(0.25f) {
    for (int i = 0; i < segmentCount; i++) {
        valid[i] = false;
        segments[i] = nullptr;
    }

    setFlag(ItemHasContents, true);
}

NineSegDigit::~NineSegDigit() {
    for (int i = 0; i < segmentCount; i++) {
        delete segments[i];
    }
}

void
NineSegDigit::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);

    if (Q_UNLIKELY(newGeometry.height() == oldGeometry.height()))
        return;

    updateSizes();

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    initSegments();

    setDigit(digit);
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
NineSegDigit::updateSizes() {
    segmentLength = (height() - segmentWidth) / 2.0f;
    setWidth(segmentLength * 1.33f + segmentLength * (qAbs(shift) * 2.0f));
}

bool
NineSegDigit::acquireSegments() {
    if (Q_UNLIKELY(segments[0]))
        return true;

    if (Q_UNLIKELY(segmentLength < 0.0f || segmentWidth < 0.0f || off < 0.0f))
        return false;

    for (int i = 0; i < segmentCount; i++) {
        segments[i] = new Segment(offColor, 600, QEasingCurve::OutExpo, this);

        QObject::connect(segments[i], &Segment::colorChanged, this, [this, i] {
            valid[i] = false;

            update();
        });
    }

    return true;
}

void
NineSegDigit::initSegments() {
    if (Q_UNLIKELY(!segments[0] || segmentLength < 0.0f || segmentWidth < 0.0f)) {
        return;
    }

    QPolygonF horPol = horizontalSegment();
    QPolygonF verPol = verticalSegment();

    segments[0]->pol() = horPol;
    segments[3]->pol() = horPol;
    segments[6]->pol() = horPol;

    segments[1]->pol() = verPol;
    segments[2]->pol() = verPol;
    segments[4]->pol() = verPol;
    segments[5]->pol() = verPol;

    segments[1]->pol().translate(segmentLength, 0.0f);
    segments[2]->pol().translate(segmentLength, segmentLength);
    segments[3]->pol().translate(0.0f,          segmentLength * 2.0f);
    segments[4]->pol().translate(0.0f,          segmentLength);
    segments[6]->pol().translate(0.0f,          segmentLength);

    segments[7]->pol() = weirdSegment();

    for (QPointF p: qAsConst(segments[7]->pol())) {
        segments[8]->pol().append(QPointF(segmentLength - p.x(), segmentLength - p.y()));
    }

    segments[8]->pol().translate(segmentWidth, segmentLength + segmentWidth);

    for (int i = 0; i < 6; i++) {
        segments[9]->pol().append({
            qCos((M_PI / 3.0f) * i) * pointSize / 2.0f,
            qSin((M_PI / 3.0f) * i) * pointSize / 2.0f
        });
    }

    segments[9]->pol().translate(
        segmentLength + (segmentWidth * 1.2f) + pointSize * 0.75f,
        segmentLength * 2.0f + segmentWidth - pointSize * 0.5f
    );

    geometryValid = false;

    update();
}

QSGNode*
NineSegDigit::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* data) {
    if (Q_UNLIKELY(!acquireSegments()))
        return nullptr;

    if (Q_UNLIKELY(!node)) {
        node = new QSGNode;

        for (int i = 0; i < segmentCount; i++) {
            auto inode = new QSGGeometryNode;
            auto geometry = new QSGGeometry(
                QSGGeometry::defaultAttributes_Point2D(),
                (i > 6 && i != 9) ? 9 : 12
            );

            geometry->setDrawingMode(QSGGeometry::DrawTriangles);
            geometry->setVertexDataPattern(QSGGeometry::DataPattern::StaticPattern);
            geometry->setIndexDataPattern(QSGGeometry::DataPattern::StaticPattern);
            geometry->markVertexDataDirty();
            geometry->markIndexDataDirty();

            inode->setGeometry(geometry);

            auto material = new QSGFlatColorMaterial;

            material->setColor(segments[i]->color());
            inode->setMaterial(material);

            inode->setFlag(QSGNode::OwnsMaterial);
            inode->setFlag(QSGNode::OwnsGeometry);

            node->appendChildNode(inode);
            node->setFlag(QSGNode::OwnedByParent);
            node->markDirty(QSGNode::DirtyNodeAdded);
        }
    }

    if (Q_UNLIKELY(!shiftValid)) {
        QSGTransformNode* tnode = data->transformNode;
        QMatrix4x4 matrix(
            1.0f, -shift, 0.0f, 0.0f,
            0.0f, 1.0f,   0.0f, 0.0f,
            0.0f, 0.0f,   1.0f, 0.0f,
            0.0f, 0.0f,   0.0f, 1.0f
        );

        matrix.translate(segmentLength * (shift * 2.0f), 0.0f);

        tnode->setMatrix(tnode->matrix() * matrix);
        tnode->markDirty(QSGNode::DirtyMatrix);

        shiftValid = true;
    }

    if (Q_UNLIKELY(!geometryValid)) {
        for (int i = 0; i < segmentCount; i++) {
            auto inode = static_cast<QSGGeometryNode*>(node->childAtIndex(i));

            QSGGeometry::Point2D* vertices = inode->geometry()->vertexDataAsPoint2D();
            QPolygonF& pol = segments[i]->pol();

            for (int k = 2, j = 0; k < pol.size(); k++, j += 3) {
                vertices[j    ].set(pol.value(0    ).x(), pol.value(0    ).y());
                vertices[j + 1].set(pol.value(k - 1).x(), pol.value(k - 1).y());
                vertices[j + 2].set(pol.value(k    ).x(), pol.value(k    ).y());
            }

            inode->markDirty(QSGNode::DirtyGeometry);
        }

        geometryValid = true;
    }

    for (int i = 0; i < segmentCount; i++) {
        if (valid[i])
            continue;

        auto inode    = static_cast<QSGGeometryNode*>(node->childAtIndex(i));
        auto material = static_cast<QSGFlatColorMaterial*>(inode->material());

        material->setColor(segments[i]->color());

        inode->markDirty(QSGNode::DirtyMaterial);

        valid[i] = true;
    }

    return node;
}


void
NineSegDigit::setDigit(const QString& digit) {
    NineSegDigit::digit = digit;

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    const int dgSgCount = segmentCount - 1;

    const bool states[12][dgSgCount + 7] = {
//     7  A  B  C  D  E  F  G  9  A  B  C  D  E  F  G  H  I
        { 1, 1, 1, 1, 1, 1, 0,    1, 1, 1, 1, 1, 1, 1, 1, 0 }, // 0
        { 0, 1, 1, 0, 0, 0, 0,    1, 0, 0, 1, 1, 0, 0, 0, 0 }, // 1
        { 1, 1, 0, 1, 1, 0, 1,    0, 0, 1, 1, 0, 1, 1, 0, 1 }, // 2
        { 1, 0, 1, 1, 0, 0, 1,    1, 0, 1, 1, 1, 1, 0, 0, 1 }, // 3
        { 0, 1, 1, 0, 0, 1, 1,    0, 0, 0, 1, 1, 0, 0, 1, 1 }, // 4
        { 1, 0, 1, 1, 0, 1, 1,    0, 0, 1, 0, 1, 1, 0, 1, 1 }, // 5
        { 1, 0, 1, 1, 1, 1, 1,    0, 0, 1, 0, 1, 1, 1, 1, 1 }, // 6
        { 1, 0, 0, 0, 0, 0, 0,    1, 1, 1, 1, 1, 0, 0, 0, 0 }, // 7
        { 1, 1, 1, 1, 1, 1, 1,    0, 0, 1, 1, 1, 1, 1, 1, 1 }, // 8
        { 1, 1, 1, 1, 0, 1, 1,    0, 0, 1, 1, 1, 1, 0, 1, 1 }, // 9
        { 0, 0, 0, 0, 0, 0, 0,    1, 1, 0, 0, 0, 0, 0, 0, 0 }, // /
        { 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0 }  //
    };

    int stateNum = 11;

    if (Q_LIKELY(digit[0].isDigit()))
        stateNum = digit[0].digitValue();
    else if (digit[0].toLatin1() == '/')
        stateNum = 10;

    for (int i = 0; i < dgSgCount; i++) {
        QColor newColor = QColorConstants::Transparent;

        if (i < 7 || nine) {
            newColor = states[stateNum][i + (nine ? 0 : dgSgCount)] ? color : offColor;
        }

        segments[i]->setEndColor(newColor);
    }

    setPoint(point);
}

void
NineSegDigit::setColor(const QColor& color) {
    NineSegDigit::color = color;

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    setDigit(digit);
}

void
NineSegDigit::setOffColor(const QColor& offColor) {
    NineSegDigit::offColor = offColor;

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    setDigit(digit);
}

void
NineSegDigit::setPoint(bool point) {
    NineSegDigit::point = point;

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    segments[9]->setEndColor(point ? color : offColor);
}

void
NineSegDigit::setSegmentWidth(float segmentWidth) {
    if (NineSegDigit::segmentWidth == segmentWidth)
        return;

    NineSegDigit::segmentWidth = segmentWidth;

    off = segmentWidth / 6.0f;
    pointSize = segmentWidth * 1.333f;
    segmentLength = (height() - segmentWidth) / 2.0f;

    if (Q_UNLIKELY(!acquireSegments()))
        return;

    initSegments();

    setDigit(digit);
}

void
NineSegDigit::setShift(float shift) {
    if (Q_UNLIKELY(NineSegDigit::shift == shift))
        return;

    NineSegDigit::shift = shift;

    shiftValid = false;

    updateSizes();

    update();
}

void
NineSegDigit::setNine(bool nine) {
    if (Q_UNLIKELY(NineSegDigit::nine == nine))
        return;

    NineSegDigit::nine = nine;

    setDigit(digit);
}
