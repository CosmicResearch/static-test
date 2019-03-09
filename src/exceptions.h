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

#include <exception>

class bad_event_exception : public std::exception {

public:
    virtual const char* what() const throw() {
        return "Event does not exist!";
    }

};

class already_started_exception : public std::exception {

public:
    virtual const char* what() const throw() {
        return "Already started!";
    }

};

class not_started_exception : public std::exception {

public:
    virtual const char* what() const throw() {
        return "Not started!";
    }

};

class stopped_exception : public std::exception {

public:
    virtual const char* what() const throw() {
        return "Stopped!";
    }

};

class bad_event_data_exception : public std::exception {

public:
    virtual const char* what() const throw() {
        return "Bad event data!";
    }

};

class db_exception : public std::runtime_error {

    using runtime_error::runtime_error;

};