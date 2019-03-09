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
#include <vector>
#include <mutex>
#include <unordered_map>

#include <nbsdx/ThreadPool.h>

#include "Listener.h"
#include "exceptions.h"
#include "LogService.h"

class Dispatcher {

private:

    std::unordered_map<std::string, std::vector<std::shared_ptr<Listener> > > listeners;

    nbsdx::concurrent::ThreadPool<THREADS> pool;

    std::atomic<bool> started, stopped;

    std::mutex mtx;

public:

    Dispatcher() : started(false), stopped(false) {
        
    }

    void start();

    void stop();

    void subscribe(const std::string& event, std::shared_ptr<Listener> listener);

    void subscribe(const std::vector<std::string>& events, std::shared_ptr<Listener> listener);

    void dispatch(std::string event, std::shared_ptr<data_base_t> data);

};