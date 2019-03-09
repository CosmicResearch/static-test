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

#include "Listener.h"
#include <atomic>
#include <iostream>

class StubListener : public Listener {

public:

    using Listener::Listener;

    virtual void start() {
        this->invoke_count = 0;
        this->started = true;
    }

    virtual void stop() {
        this->started = false;
    }

    virtual void execute(Event e, Variant data) {
        std::unique_lock<std::mutex> lck(mtx);
        invoke_count++;
        lastEvent = e;
        lastData = data;
    }

    int getCount() {
        std::unique_lock<std::mutex> lck(mtx);
        return invoke_count;
    }

    Variant getLastData() {
        std::unique_lock<std::mutex> lck(mtx);
        return lastData;
    }

    Event getLastEvent() {
        std::unique_lock<std::mutex> lck(mtx);
        return lastEvent;
    }

private:
    std::atomic_int invoke_count;
    Variant lastData;
    Event lastEvent;
    std::mutex mtx;

};