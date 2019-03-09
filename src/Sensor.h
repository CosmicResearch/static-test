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

#include <string>
#include <cstdlib>
#include <mutex>
#include "Configuration.h"
#include "Dispatcher.h"
#include "LogService.h"
#include "Thread.h"
#include <atomic>

class Sensor {

public:

    Sensor(Configuration& configuration) : stopped(false), started(false), configuration(configuration) {
    
    }

    virtual void start();

    virtual void fetch(Dispatcher* dispatcher) = 0;

    virtual void stop();

protected:

    virtual void read() = 0;

    int64_t samplingRate;

    Configuration configuration;

    thread read_thread;

private:

    void threadFunction();

    std::atomic<bool> stopped, started;

};