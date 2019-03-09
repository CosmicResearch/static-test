/**
 *   Copyright (C) 2018  Guillem Castro
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Sensor.h"

void Sensor::start() {

    this->read_thread = thread(&Sensor::threadFunction, this);
    int priority = this->read_thread.getMaxSchedulingPriority(SCHED_RR);
    this->read_thread.setScheduling(SCHED_RR, priority);
    started = true;

}

void Sensor::stop() {
    this->stopped = true;
}

void Sensor::threadFunction() {

    while (!started) {
        
    }

    while (!stopped) {

        this->read();

        this_thread::high_precision_sleep_for(this->samplingRate);

    }

}