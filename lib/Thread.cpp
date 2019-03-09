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

#include "Thread.h"

void thread::setScheduling(int policy, int priority) {
    sched_param params;
    params.sched_priority = priority;
    if (pthread_setschedparam(this->native_handle(), policy, &params)) {
        throw scheduler_exception(std::strerror(errno));
    }
}

int thread::getMaxSchedulingPriority(int policy) {
    return sched_get_priority_max(policy);
}

void this_thread::high_precision_sleep_for(int64_t nsecs) {
    timespec tim, tim2;
    tim.tv_sec = nsecs / 1000000000;
    tim.tv_nsec = nsecs % 1000000000;
    nanosleep(&tim, &tim2);
}