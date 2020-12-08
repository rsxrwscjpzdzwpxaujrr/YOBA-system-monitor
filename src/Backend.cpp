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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include <QThread>
#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent), cpuLoad(0.0f) {}

void
Backend::start() {
    QThread* thread = QThread::create([this] {
        while(true) {
            FILE* file;

            float a[4];
            float b[4];

            char fileName[] = "/proc/stat";
            char formatString[] = "%*s %f %f %f %f";

            file = fopen(fileName, "r");
            fscanf(file, formatString, &a[0], &a[1], &a[2], &a[3]);
            fclose(file);

            sleep(1);

            file = fopen(fileName, "r");
            fscanf(file, formatString, &b[0], &b[1], &b[2], &b[3]);
            fclose(file);

            cpuLoad = ((b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2])) /
                      ((b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3]));

            emit cpuLoadChanged();
        }
    });

    thread->start();
}

float
Backend::getCpuLoad() {
    return cpuLoad;
}

unsigned long
Backend::getUsedRam() {
    unsigned long usedRam;
    FILE* file;

    file = fopen("/proc/meminfo", "r");
    fscanf(file, "%*[^\n]\n%*[^\n]\nMemAvailable:%*[ ]%lu kB", &usedRam);
    fclose(file);

    usedRam = getTotalRam() - (usedRam * 1024);

    return usedRam;
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
