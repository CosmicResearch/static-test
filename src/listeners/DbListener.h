/**
 *   Copyright (C) 2019  Guillem Castro
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

#include "Listener.h"
#include "Thread.h"
#include <queue>
#include <utility>
extern "C" {
#include "sqlite3.h"
}
#include "exceptions.h"
#include "LogService.h"

class DbListener : public Listener {

    using Listener::Listener;

public:

    virtual void start() override;

    virtual void stop() override;

    virtual void execute(std::string event, std::shared_ptr<data_base_t> data) override;

private:

    thread db_thread;

    std::queue< std::pair<std::shared_ptr<analog_read_t>, std::string> > data;

    void threadFunc();

    void open();

    void close();

    void insert(std::string event, std::shared_ptr<analog_read_t> data);

    sqlite3* db;

    sqlite3_stmt* insert_statement;

};