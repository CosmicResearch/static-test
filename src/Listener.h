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

#include "Configuration.h"
#include "data_types.h"
#include <atomic>

//Forward declaration
class Dispatcher;

/**
 * A Listener for an Event that will be registered in a Dispatcher for receiving events notifications.
 */ 
class Listener {

public:

    /**
     * Constructor to build a new Listener with a pointer to the Dispatcher. Make shure to
     * call this constructor in derived classes.
     * 
     * The best way to inherit this constructor is using the declaration
     * `using Listener::Listener` in the class declaration.
     * 
     * @params d A pointer to a Dispatcher
     */
    Listener(Dispatcher* dispatcher) : dispatcher(dispatcher), started(false), stopped(false) {

    }

    /**
     * Constructor to build a new Listener with a pointer to the Dispatcher. Make shure to
     * call this constructor in derived classes if you need to access application configuration.
     * 
     * The best way to inherit this constructor is using the declaration
     * `using Listener::Listener` in the class declaration.
     * 
     * @params d A pointer to a Dispatcher
     * @params
     */
    Listener(Dispatcher* dispatcher, Configuration& configuration) : dispatcher(dispatcher), config(configuration) {

    }

    /**
     * Start the Listener. This method will be called after the Listener has been subscribed to a Dispatcher.
     * After calling this method, the Listener should be ready to receive events.
     */
    virtual void start() = 0;

    /**
     * Stops the Listener. After calling this method, the Listener must guarantee that all invocations to execute()
     * are ignored.
     */
    virtual void stop() = 0;

    /**
     * Get the current state of the Listener, i.e. if it has been started or not. If the Listener was stopped, this
     * method must return false.
     * @returns If the Listener has been started or not.
     */
    virtual bool isStarted() {
        return started;
    }

    /**
     * Method to receive the notification of the event and its data.
     * @params event The event code of the event being notified.
     * @params data  The data that generated the event. An event with a null data must not be ignored.
     */
    virtual void execute(std::string event, std::shared_ptr<data_base_t> data) = 0;

protected:

    std::atomic<bool> started;
    std::atomic<bool> stopped;

    Dispatcher* dispatcher;
    Configuration config;

};