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

#include "Backend.hpp"

#include <cstdio>
#include <sys/sysinfo.h>

#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent) {
    getCpuLoad();
}

float
Backend::getCpuLoad() {
    unsigned long temp[4];

    unsigned long oldLoad = load;
    unsigned long oldTotal = total;

    std::FILE* file = std::fopen("/proc/stat", "r");
    std::fscanf(file, "%*s %ld %ld %ld %ld", &temp[0], &temp[1], &temp[2], &temp[3]);
    std::fclose(file);

    load = temp[0] + temp[1] + temp[2];
    total = load + temp[3];

    return static_cast<float>(load - oldLoad) / (total - oldTotal);
}

unsigned long
Backend::getUsedRam() {
    unsigned long usedRam;

    std::FILE* file = std::fopen("/proc/meminfo", "r");
    std::fscanf(file, "%*[^\n]\n%*[^\n]\nMemAvailable:%*[ ]%lu kB", &usedRam);
    std::fclose(file);

    return getTotalRam() - (usedRam * 1024);
}

unsigned long
Backend::getTotalRam() {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        return info.totalram;
    } else {
        qDebug() << "RAM getting error";
        return 0;
    }
}
