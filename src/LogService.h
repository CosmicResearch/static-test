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

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

#include <string>
#include <iostream>

class LogService {

public:
    void initializeLogging(const std::string& logPrefix, const std::string& logFilePath, bool enableConsole);

private:

    std::unique_ptr<g3::LogWorker> logWorker;
};

struct ConsoleSink {

    enum FG_Color {YELLOW = 33, RED = 31, GREEN=32, WHITE = 97};

    FG_Color GetColor(const LEVELS level) const {
        if (level.value == WARNING.value) { return YELLOW; }
        if (level.value == DEBUG.value) { return GREEN; }
        if (g3::internal::wasFatal(level)) { return RED; }
        return WHITE;
    }
  
    void ReceiveLogMessage(g3::LogMessageMover logEntry) {
        auto level = logEntry.get()._level;
        auto color = GetColor(level);

        std::cout << "\033[" << color << "m" << logEntry.get().toString() << "\033[m" << std::endl;
    }

};