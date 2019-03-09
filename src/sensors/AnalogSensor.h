/**
 *   Copyright (C) 2019  Guillem Castro, Joan Mitjans
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

#include <Sensor.h>
#include <queue>
#include <fcntl.h>
#include <chrono>
#include <unistd.h>
#include <cstdlib>
#include <LogService.h>
#include <fstream>
#include <iostream>

class AnalogSensor : public Sensor {

public:

    using Sensor::Sensor;

    AnalogSensor(Configuration& configuration) : Sensor(configuration) {
        readings = std::queue<std::shared_ptr<analog_read_t> >();
        errors = std::queue<std::shared_ptr<error_data_t> >();
    }

    virtual void start() override;

    virtual void fetch(Dispatcher* dispatcher) override;

protected:

    virtual void read() override;

private:

    std::queue<std::shared_ptr<analog_read_t> > readings;
    std::queue<std::shared_ptr<error_data_t> > errors;

    std::string file;

    double zeroValue;

    double spanValue;

    double scale;

    int64_t quantizationBits;

    double lowVoltage;

    double spanVoltage;

    double span, zero;

    std::string event;

    std::string name;

};