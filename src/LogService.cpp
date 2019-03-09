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

#include "LogService.h"

void LogService::initializeLogging(const std::string& logPrefix, const std::string& logFilePath, bool enableConsole) {
    logWorker = std::unique_ptr<g3::LogWorker> {g3::LogWorker::createLogWorker()};
    auto fileSinkHandle = logWorker->addSink(std::make_unique<g3::FileSink>(logPrefix, logFilePath), &g3::FileSink::fileWrite);
    if (enableConsole) {
        auto consoleSinkHandle = logWorker->addSink(std::make_unique<ConsoleSink>(), &ConsoleSink::ReceiveLogMessage);
    }
    g3::initializeLogging(logWorker.get());
}