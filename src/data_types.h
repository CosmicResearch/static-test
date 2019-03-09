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

#include <cstdint>
#include <cstring>
#include <string>

#ifndef double
#endif

struct data_base_t {

    uint64_t time;

    std::string origin;

    data_base_t() {
        this->time = 0;
    }

    virtual bool pack(char buff[], size_t& size) {
        return false;
    }

};

struct error_data_t : data_base_t {

    std::string msg;

};

struct mag_data_t : data_base_t {

    int16_t x;
    int16_t y;
    int16_t z;
    int16_t u_temp;

    mag_data_t() {
        this->time = 0;
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->u_temp = 0;
    }

    bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->x) + sizeof(this->y) + sizeof(this->z);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->x, sizeof(this->x));
        i += sizeof(this->x);
        memcpy(&buff[i], &this->y, sizeof(this->y));
        i += sizeof(this->y);
        memcpy(&buff[i], &this->z, sizeof(this->z));
        i += sizeof(this->z);
        size = i;
        return true;
    }

};

struct accel_data_t : data_base_t {

    int16_t x;
    int16_t y;
    int16_t z;

    accel_data_t() {
        this->time = 0;
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

     bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->x) + sizeof(this->y) + sizeof(this->z);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->x, sizeof(this->x));
        i += sizeof(this->x);
        memcpy(&buff[i], &this->y, sizeof(this->y));
        i += sizeof(this->y);
        memcpy(&buff[i], &this->z, sizeof(this->z));
        i += sizeof(this->z);
        size = i;
        return true;
    }
};

struct bar_data_t : data_base_t {

    double pressure;
    double temperature;
    int32_t altitude;

    bar_data_t() {
        this->time = 0;
        this->pressure = 0;
        this->temperature = 0;
        this->altitude = 0;
    }

     bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->pressure) + sizeof(this->temperature) + sizeof(this->altitude);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->pressure, sizeof(this->pressure));
        i += sizeof(this->pressure);
        memcpy(&buff[i], &this->temperature, sizeof(this->temperature));
        i += sizeof(this->temperature);
        memcpy(&buff[i], &this->altitude, sizeof(this->altitude));
        i += sizeof(this->altitude);
        size = i;
        return true;
    }
};

enum gps_type {
    GGA = 0,
    RMC,
    GLL,
};

struct gps_data_t : data_base_t {

    float speed, angle, magvariation, HDOP;
    uint8_t hour, minute, seconds;
    uint8_t day, month;
    uint16_t year;
    uint32_t latitude, longitude; //Stored in units of 1/1000000 degrees
    char latitudeChar, longitudeChar;
    int32_t altitude; //Stored in units of 1/100 meters
    bool fix;
    uint8_t fixQuality, satellites;
    gps_type type;

    gps_data_t() {
        this->time = 0;
        this->speed = this->angle = this->magvariation = this->HDOP = 0.f;
        this->hour = this->minute = this->seconds = 0;
        this->day = this->month = 0;
        this->year = 0;
        this->latitude = this->longitude = 0;
        this->latitudeChar = this->longitudeChar = 0;
        this->altitude = -1;
        this->fix = false;
        this->fixQuality = this->satellites = 0;
        this->type = gps_type::GGA;
    }

    virtual bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->altitude) + sizeof(this->latitude) + sizeof(this->longitude) + sizeof(this->latitudeChar) + sizeof(this->longitudeChar);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->altitude, sizeof(this->altitude));
        i += sizeof(this->altitude);
        memcpy(&buff[i], &this->latitude, sizeof(this->latitude));
        i += sizeof(this->latitude);
        memcpy(&buff[i], &this->longitude, sizeof(this->longitude));
        i += sizeof(this->longitude);
        memcpy(&buff[i], &this->latitudeChar, sizeof(this->latitudeChar));
        i += sizeof(this->latitudeChar);
        memcpy(&buff[i], &this->longitudeChar, sizeof(this->longitudeChar));
        i += sizeof(this->longitudeChar);
        memcpy(&buff[i], &this->fix, sizeof(this->fix));
        i += sizeof(this->fix);
        size = i;
        return true;
    }

};

struct gyro_data_t : data_base_t {

    int16_t x;
    int16_t y;
    int16_t z;

    gyro_data_t() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    virtual bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->x) + sizeof(this->y) + sizeof(this->z);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->x, sizeof(this->x));
        i += sizeof(this->x);
        memcpy(&buff[i], &this->y, sizeof(this->y));
        i += sizeof(this->y);
        memcpy(&buff[i], &this->z, sizeof(this->z));
        i += sizeof(this->z);
        size = i;
        return true;
    }

};

struct orientation_data_t : data_base_t {
    double pitch, roll, heading;
    double i, j, k, u;

    orientation_data_t() {

    }

    virtual bool pack(char buff[], size_t& size) {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->pitch) + sizeof(this->roll) + sizeof(this->heading) +
            sizeof(i) + sizeof(j) + sizeof(k) + sizeof(u);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->pitch, sizeof(this->pitch));
        i += sizeof(this->pitch);
        memcpy(&buff[i], &this->roll, sizeof(this->roll));
        i += sizeof(this->roll);
        memcpy(&buff[i], &this->heading, sizeof(this->heading));
        i += sizeof(this->heading);
        memcpy(&buff[i], &this->i, sizeof(this->i));
        i += sizeof(this->i);
        memcpy(&buff[i], &this->j, sizeof(this->j));
        i += sizeof(this->j);
        memcpy(&buff[i], &this->k, sizeof(this->k));
        i += sizeof(this->k);
        memcpy(&buff[i], &this->u, sizeof(this->u));
        i += sizeof(this->u);
        size = i;
        return true;
    }

};

struct imu_data_t : data_base_t {

    accel_data_t accel_data;
    gyro_data_t gyro_data;
    mag_data_t mag_data;

    imu_data_t() {

    }

    virtual bool pack(char buff[], size_t& size) {
        size_t accel_size, gyro_size, mag_size;
        bool ret = true;
        ret &= accel_data.pack(buff, accel_size);
        ret &= gyro_data.pack(buff + accel_size, gyro_size);
        ret &= mag_data.pack(buff + accel_size + gyro_size, mag_size);
        size = accel_size + gyro_size + mag_size;
        return ret;
    }

};

struct analog_read_t : public data_base_t {

    double value;

    analog_read_t() {
        this->value = 0.0;
    }

    virtual bool pack(char buff[], size_t& size) override {
        uint16_t i = 0;
        uint8_t data_size = sizeof(this->time) + sizeof(this->value);
        buff[i] = data_size;
        i += sizeof(data_size);
        memcpy(&buff[i], &this->time, sizeof(this->time));
        i += sizeof(this->time);
        memcpy(&buff[i], &this->value, sizeof(this->value));
        i += sizeof(this->value);
        size = i;
        return true;
    }

};