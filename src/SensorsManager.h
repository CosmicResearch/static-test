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

#include <memory>
#include "LogService.h"
#include "Thread.h"
#include "Sensor.h"
#include "Dispatcher.h"

#include <nbsdx/ThreadPool.h>

#include <mutex>
#include <atomic>
#include <chrono>
#include <unordered_map>

class SensorsManager {

public:

    SensorsManager() : started(false), stopped(false) {
        
    }

    explicit SensorsManager(Dispatcher* dispatcher) : dispatcher(dispatcher), started(false), stopped(false) {

    }

    SensorsManager(Dispatcher* dispatcher, Configuration& config) : dispatcher(dispatcher), started(false), stopped(false), config(config) {

    }

    void addSensor(std::shared_ptr<Sensor> sensor);

    void start();

    void stop();

    void setDispatcher(Dispatcher* dispatcher) {
        this->dispatcher = dispatcher;
    }

private:

    void run();

    std::vector<std::shared_ptr<Sensor> > sensors;
    
    Dispatcher* dispatcher;

    bool started;

    thread sensor_thread;

    std::mutex mtx;

    std::atomic<bool> stopped;

    nbsdx::concurrent::ThreadPool<THREADS> pool;

    Configuration config;

};