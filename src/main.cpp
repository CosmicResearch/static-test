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

#ifndef TESTS

#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <iostream>

#include "Application.h"

void signal_handler(int signo);

void print_usage();

Application* app;

bool stop = false;

int main(int argc, char* argv[]) {

    int c;
    bool is_daemon = false;
    const char* config_file;
    bool is_config_file = false;
    bool is_debug = false;

    while((c = getopt(argc, argv, "dhc:D")) != -1) {
        switch(c) {
            case 'D':
                is_daemon = true;
                break;
            case 'h':
                print_usage();
                exit(0);
                break;
            case 'c':
                if (optarg) {
                    config_file = optarg;
                    is_config_file = true;
                }
                break;
            case 'd':
                is_debug = true;
                break;
        }

    }

    if (!is_config_file) {
        std::cerr << "A configuration file must be specified!" << std::endl;
        print_usage();
        exit(2);
    }

    std::cout << "Using configuration file \"" << config_file << "\"" << std::endl;

    //Capture Control+C
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        std::cerr << "ERROR: Cant set SIGINT signal handler. Exiting." << std::endl;
        exit(2);
    }
    //Start the application as a daemon (background task)
    if (is_daemon) {
        std::cout << "Setting as daemon" << std::endl;
        if (daemon(0, 0) != 0) {
            std::cerr << "Error setting as daemon. Exiting." << std::endl;
            exit(2);
        }
    }

    std::cout << "Create config" << std::endl;

    Configuration config;
    config.parse(config_file);
    //config.setArgv(argc, argv);

    std::cout << "Set debug" << std::endl;

    config["debug"].set<bool>(is_debug);

    std::cout << "Create app" << std::endl;

    app = new Application(config);

    app->setup();
    while (!stop) {
        app->run();
    }
    app->stop();
}


void signal_handler(int signo) {
    stop = true;
}

void print_usage() {
    std::cout << "Usage: avionics -c config_file [-hd]" << std::endl;
}

#endif