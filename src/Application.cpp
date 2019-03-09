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

#include "Application.h"
//#include "StdoutPrinter.h"

#include "LogService.h"

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>

//Default number of threads
#ifndef THREADS
    #define THREADS 4
#endif

Application::Application(Configuration config) : config(config) {

}

/**
 * Setup the application, parse the configuration and initialize the Dispatcher and Listeners.
 */
void Application::setup() {

    std::cout << "Starting application 1" << std::endl;

    std::string log_dir = config["log_dir"].get<std::string>();

    std::cout << "Starting application 2, log_dir was " << log_dir << std::endl;

    bool debug = config["debug"].get<bool>();

    logService.initializeLogging("unknown", log_dir, debug);

    manager.setDispatcher(&dispatcher);
    manager.addSensor(std::make_shared<AnalogSensor>(config["thermocoupler_1"]));
    manager.addSensor(std::make_shared<AnalogSensor>(config["thermocoupler_2"]));
    manager.addSensor(std::make_shared<AnalogSensor>(config["thermocoupler_3"]));
    manager.addSensor(std::make_shared<AnalogSensor>(config["load_cell"]));
    manager.addSensor(std::make_shared<AnalogSensor>(config["pressure"]));

    std::shared_ptr<Listener> dblistener = std::make_shared<DbListener>(&dispatcher);
    try {
        dispatcher.subscribe("read_thermocoupler", dblistener);
        dispatcher.subscribe("read_load_cell", dblistener);
        dispatcher.subscribe("read_pressure", dblistener);
    }
    catch(std::exception& ex) {
        LOG(WARNING) << ex.what();
    }

    try {
        dispatcher.start();
        manager.start();
    }
    catch(std::exception& ex) {
        LOG(WARNING) << ex.what();
    }

    LOG(INFO) << "Application started successfully";
}

/**
 * A function that will be called in a loop. Best for testing and debugging. Not to be used in production.
 * This function must return once per loop.
 */
void Application::run() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

/**
 * Stop the application. Save context (TODO) and stop Dispatcher.
 */
void Application::stop() {
    LOG(INFO) << "Stopping application...";
    dispatcher.stop();
    manager.stop();
}