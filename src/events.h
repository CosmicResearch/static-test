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

#define EVENT_UNDEFINED "undefined"

#define EVENT_READ_GPS "read_gps"
#define EVENT_READ_ACCELEROMETER "read_accelerometer"
#define EVENT_READ_MAGNETOMETER "read_magnetometer"
#define EVENT_READ_GYROSCOPE "read_gyroscope"
#define EVENT_READ_BAROMETER "read_barometer"
#define EVENT_READ_IMU "read_imu"

#define EVENT_APOGEE  "apogee"
#define EVENT_MAIN_FIRED "main_fired"
#define EVENT_DROGUE_FIRED "drogue_fired"
#define EVENT_SENSOR_INIT "sensor_init"
#define EVENT_LIFTOFF "liftoff"

#define EVENT_ERROR "error"
#define EVENT_ERROR_SENSOR_INIT "error_sensor_init"
#define EVENT_ERROR_SENSOR_READ "error_sensor_read"
#define EVENT_ERROR_PILOT "error_pilot"
#define EVENT_ERROR_MAIN "error_main"