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
#include <QObject>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(unsigned long totalRam READ getTotalRam CONSTANT)
    QML_ELEMENT

public:
    explicit Backend(QObject* parent=nullptr);
    ~Backend() override;

    Q_INVOKABLE unsigned long getTotalRam();

public slots:
    float getCpuLoad();
    unsigned long getUsedRam();

private:
    QFile proc_stat;
    QFile meminfo;

    unsigned long load;
    unsigned long total;
};
