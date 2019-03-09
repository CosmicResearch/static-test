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

#include "StdoutPrinter.h"
#include <thread>

void StdoutPrinter::start() {
    this->started = true;
}

void StdoutPrinter::stop() {
    this->started = false;
}

void StdoutPrinter::execute(std::string e, std::shared_ptr<data_base_t> data) {
    std::cout << "-----------------" << std::endl;
    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result)) << result << std::endl;
    std::cout << "Received event with code " << e << std::endl;
    std::cout << "In thread with ID " << std::this_thread::get_id() << std::endl;
    if (e == "read_thermocoupler" || e == "read_load_cell" || e == "read_pressure") {
        std::shared_ptr<analog_read_t> analog_read = std::dynamic_pointer_cast<analog_read_t>(data);
        std::cout << analog_read->origin << " sensor value: " << analog_read->value << std::endl;
    }
}