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

#include "AnalogSensor.h"

void AnalogSensor::start() {
    file = configuration["file"].get<std::string>();
    LOG(DEBUG) << "file: " << file;
    event = configuration["event"].get<std::string>();
    name = configuration["name"].get<std::string>();
    samplingRate = configuration["sampling_rate"].get<int64_t>();
    zeroValue = configuration["zero"].get<double>();
    spanValue = configuration["span"].get<double>();
    scale = configuration["scale"].get<double>();
    quantizationBits = configuration["adc"]["quantization_bits"].get<int64_t>();
    lowVoltage = configuration["adc"]["low_voltage"].get<double>();
    spanVoltage = configuration["adc"]["span_voltage"].get<double>();
    zero = (double)(1 << quantizationBits) * (zeroValue - lowVoltage) / spanVoltage;
    span = (double)(1 << quantizationBits) * (spanValue) / spanVoltage;
    Sensor::start();
}

void AnalogSensor::read() {
    std::string buff;
    std::shared_ptr<analog_read_t> value = std::make_shared<analog_read_t>();
    auto now = std::chrono::high_resolution_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto timeValue = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    uint64_t time = timeValue.count();
    std::ifstream fp(this->file);
    if (fp.fail()) {
        LOG(WARNING) << "While trying to open sensor " << name;
        std::ostringstream stringStream;
        stringStream << "Cannot open file " << this->file;
        std::string msg = stringStream.str();
        std::shared_ptr<error_data_t> error = std::make_shared<error_data_t>();
        error->msg = msg;
        error->time = time;
        errors.push(error);
    }
    fp >> buff;
    if (fp.fail()) {
        LOG(WARNING) << "While trying to read sensor " << name;
        std::ostringstream stringStream;
        stringStream << "Cannot read file " << this->file;
        std::string msg = stringStream.str();
        std::shared_ptr<error_data_t> error = std::make_shared<error_data_t>();
        error->msg = msg;
        error->time = time;
        errors.push(error);
    }
    LOG(DEBUG) << buff;
    value->value = (std::stoi(buff) - zero) * (scale / span);
    value->time = time;
    value->origin = name;
    readings.push(value);
}

void AnalogSensor::fetch(Dispatcher* dispatcher) {
    while (!errors.empty()) {
        dispatcher->dispatch("error_sensor_read", errors.front());
        errors.pop();
    }
    while (!readings.empty()) {
        dispatcher->dispatch(event, readings.front());
        readings.pop();
    }
}