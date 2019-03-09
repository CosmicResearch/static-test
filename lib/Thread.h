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

#pragma once

#include <thread>
#include <pthread.h>
#include <cstring>
#include <stdexcept>
#include <ctime>
#include <sched.h>

class thread : public std::thread {

    using std::thread::thread;

public:

    void setScheduling(int policy, int priority);

    static int getMaxSchedulingPriority(int policy);

};

class scheduler_exception : public std::runtime_error {
    using runtime_error::runtime_error;
};

namespace this_thread {

    void high_precision_sleep_for(int64_t nsecs);

}