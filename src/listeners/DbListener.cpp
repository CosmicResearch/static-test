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

#include "DbListener.h"

void DbListener::start() {
    if (started) {
        return;
    }
    open();
    this->db_thread = thread(&DbListener::threadFunc, this);
    this->started = true;
}

void DbListener::open() {
    int error = sqlite3_open("database.db", &this->db);
    if (error != SQLITE_OK) {
        const char* msg = sqlite3_errmsg(this->db);
        throw new db_exception(msg);
    }
    const char* insert_sql = "INSERT INTO readings (event, origin, time, value) VALUES (?1, ?2, ?3, ?4)";
    int rc = sqlite3_prepare_v2(this->db, insert_sql, -1, &insert_statement, NULL);
    if (rc != SQLITE_OK) {
        const char* msg = sqlite3_errmsg(this->db);
        throw new db_exception(msg);
    }
    char* msg;
    rc = sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &msg);
    if (rc != SQLITE_OK) {
        LOG(WARNING) << msg;
        sqlite3_free(msg);
    }
    rc = sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &msg);
    if (rc != SQLITE_OK) {
        LOG(WARNING) << msg;
        sqlite3_free(msg);
    }
    started = true;
}

void DbListener::stop() {
    if (stopped) {
        return;
    }
    close();
    this->stopped = true;
}

void DbListener::close() {
    int rc = sqlite3_close(this->db);
    if (rc != SQLITE_OK) {
        const char* msg = sqlite3_errmsg(this->db);
        throw new db_exception(msg);
    }
    rc = sqlite3_finalize(insert_statement);
    started = false;
}

void DbListener::execute(std::string event, std::shared_ptr<data_base_t> data) {
    if (event == "read_thermocoupler" || event == "read_load_cell" || event == "read_pressure") {
        std::shared_ptr<analog_read_t> analog_read = std::dynamic_pointer_cast<analog_read_t>(data);
        this->data.push(  std::make_pair(analog_read, event) );
    }
}

void DbListener::insert(std::string event, std::shared_ptr<analog_read_t> data) {
    sqlite3_bind_text(insert_statement, 1, event.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insert_statement, 2, data->origin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(insert_statement, 3, data->time);
    sqlite3_bind_double(insert_statement, 4, data->value);
    int rc = sqlite3_step(insert_statement);
    if (rc != SQLITE_DONE) {
        const char* msg = sqlite3_errmsg(this->db);
        throw new db_exception(msg);
    }
    rc = sqlite3_reset(insert_statement);
    if (rc != SQLITE_OK) {
        const char* msg = sqlite3_errmsg(this->db);
        throw new db_exception(msg);
    }
}


void DbListener::threadFunc() {

    while (!started) {

    }

    int count = 0;
    int maxCount = 500;
    int rc;
    char* msg;

    while (!stopped) {
        rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &msg);
        if (rc != SQLITE_OK) {
            LOG(WARNING) << msg;
            sqlite3_free(msg);
        }

        while (!data.empty() && count < maxCount) {
            auto pair = data.front();
            try {
                insert(pair.second, pair.first);
            }
            catch (db_exception& exception) {
                LOG(WARNING) << exception.what();
            }
            data.pop();
            ++count;
        }

        rc = sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &msg);
        if (rc != SQLITE_OK) {
            LOG(WARNING) << msg;
            sqlite3_free(msg);
        }

        count = 0;
        this_thread::high_precision_sleep_for(100000000); //100ms
    }

}