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

#include "Dispatcher.h"

/**
 * Starts all listeners
 * @throws already_started_exception If the dispatcher has been already started.
 */
void Dispatcher::start() {

    if (this->started) {
        throw already_started_exception();
    }

    for (auto& it : listeners) {
        for (auto& listener : it.second) {
            listener->start();
        }
    }
    this->stopped = false;
    this->started = true;

}

/**
 * Stop the execution of Listeners and the dispatching of events.
 * @throws not_started_exception If the dispatcher has not been started yet.
 */
void Dispatcher::stop() {
    
    if (!this->started) {
        throw not_started_exception();
    }
    if (this->stopped) {
        return;
    }

    this->stopped = true;

    LOG(INFO) << "Stopping Dispatcher execution threads";

    pool.JoinAll();

    LOG(INFO) << "Dispatcher stopped";
}

/**
 * Subscribe a new listener to an event. When the event is dispatched, the listener will be notified.
 * @param event     The event code the listener will be subscribed to.
 * @param listener  A reference to the listener that will be subscribed.
 * @throws bad_event_exception If the event code is less than 0 or greater than N_MAX_EVENTS.
 * @throws stopped_exception   If the dispatcher was stopped.
 */
void Dispatcher::subscribe(const std::string& event, std::shared_ptr<Listener> listener) {

    if (this->stopped) {
        throw stopped_exception();
    }

    //To prevent subscribing a new listener while an event is being dispatched
    std::unique_lock<std::mutex> lck(mtx);

    this->listeners[event].emplace_back(listener);

    if (this->started) {
        listener.get()->start();
    }

    LOG(INFO) << "Listener registered for event " << event;

}

void Dispatcher::subscribe(const std::vector<std::string>& events, std::shared_ptr<Listener> listener) {

    if (this->stopped) {
        throw stopped_exception();
    }

    std::unique_lock<std::mutex> lck(mtx);

    for (std::string event : events) {

        this->listeners[event].push_back(listener);

        if (this->started) {
            listener.get()->start();
        }

        LOG(INFO) << "Listener registered for event " << event;

    }
}

/**
 * Dispatch an async event. All `Listeners` registered for that event will be notified.
 * @param event The event code to dispatch
 * @param data  The data that will be passed to all listeners
 * @throws bad_event_exception     If the event code is less than 0 or greater than N_MAX_EVENTS
 * @throws stopped_exception       If the dispatcher was stopped.
 * @throws not_started_exception   If the dispatcher has not been started yet.
 */
void Dispatcher::dispatch(std::string event, std::shared_ptr<data_base_t> data) {

    if (this->stopped) {
        throw stopped_exception();
    }
    if (!this->started) {
        throw not_started_exception();
    }

    LOG(DEBUG) << "Dispatching event " << event;

    //To prevent subscribing a new listener while an event is being dispatched
    std::unique_lock<std::mutex> lck(mtx);

    for (auto& listener : listeners[event]) {
        if (listener->isStarted()) {
            pool.AddJob([listener, event, data]() {
                try {
                    listener->execute(event, data);
                }
                //Just in case, although it should not happen
                catch (const stopped_exception& ex) {
                    LOG(WARNING) << "Tried to invoke stopped Listener for event " << event << ex.what();
                }
            });
        }
        else {
            LOG(WARNING) << "Tried to invoke non-started Listener for event " << event;
        }
    }
}