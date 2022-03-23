/*
 * Copyright (c) 2020, 2022 Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include "Backend.hpp"

#include <cstdio>
#include <sys/sysinfo.h>

#include <QDebug>

#define BUFFER_SIZE 256

Backend::Backend(QObject *parent) :
            QObject(parent),
            proc_stat("/proc/stat"),
            meminfo("/proc/meminfo") {
    proc_stat.open(QFile::ReadOnly);
    meminfo.open(QFile::ReadOnly);

    getCpuLoad();
}

Backend::~Backend() = default;

float
Backend::getCpuLoad() {
    char buffer[BUFFER_SIZE];

    unsigned long temp[4];

    unsigned long oldLoad = load;
    unsigned long oldTotal = total;

    proc_stat.read(buffer, BUFFER_SIZE);
    proc_stat.flush();
    proc_stat.seek(0);

    std::sscanf(buffer, "%*s %ld %ld %ld %ld", &temp[0], &temp[1], &temp[2], &temp[3]);

    load = temp[0] + temp[1] + temp[2];
    total = load + temp[3];

    return static_cast<float>(load - oldLoad) / (total - oldTotal);
}

unsigned long
Backend::getUsedRam() {
    char buffer[BUFFER_SIZE];

    unsigned long usedRam;

    meminfo.read(buffer, BUFFER_SIZE);
    meminfo.flush();
    meminfo.seek(0);

    std::sscanf(buffer, "%*[^\n]\n%*[^\n]\nMemAvailable:%*[ ]%lu kB", &usedRam);

    return getTotalRam() - (usedRam * 1024);
}

unsigned long
Backend::getTotalRam() {
    struct sysinfo info;

    if (Q_LIKELY(sysinfo(&info) == 0)) {
        return info.totalram;
    } else {
        qDebug() << "RAM getting error";
        return 0;
    }
}
