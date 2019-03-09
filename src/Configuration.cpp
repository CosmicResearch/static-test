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

#include "Configuration.h"
#include <iostream>

#include "LogService.h"

Configuration::Configuration() {
    this->childs = std::unordered_map<std::string, std::shared_ptr<Configuration> >();
    this->content = nullptr;
}

Configuration::Configuration(std::string file) : Configuration() {
    this->parse(file);
}

Configuration::Configuration(json& json_file) : file(json_file) {
    this->childs = std::unordered_map<std::string, std::shared_ptr<Configuration> >();
    this->content = nullptr;
}

Configuration::Configuration(std::shared_ptr<ConfigurationContentBase> content) : content(content) {
    this->childs = std::unordered_map<std::string, std::shared_ptr<Configuration> >();
}

Configuration::Configuration(json& json_file, std::shared_ptr<ConfigurationContentBase> content) : file(json_file), content(content) {
    this->childs = std::unordered_map<std::string, std::shared_ptr<Configuration> >();
}

Configuration::Configuration(const Configuration& other) : childs(other.childs), content(other.content), file(other.file) {

}

void Configuration::parse(std::string file) {
    std::ifstream i(file);
    this->file = nlohmann::json::parse(i);
}

Configuration& Configuration::at(const std::string property) {
    if (childs.find(property) == childs.end()) {
        if (file.find(property) != file.end()) {
            if (not file[property].is_object() and not file[property].is_array()) {
                std::shared_ptr<ConfigurationContentBase> new_content;
                if (file[property].is_boolean()) {
                    new_content = std::make_shared<ConfigurationContent<bool> >(file[property].get<bool>());
                }
                else if (file[property].is_string()) {
                    new_content = std::make_shared<ConfigurationContent<std::string> >(file[property].get<std::string>());
                }
                else if (file[property].is_number_float()) {
                    new_content = std::make_shared<ConfigurationContent<double> >(file[property].get<double>());
                }
                else if (file[property].is_number_integer()) {
                    new_content = std::make_shared<ConfigurationContent<int64_t> >(file[property].get<int64_t>());
                }
                else if (file[property].is_number_unsigned()) {
                    new_content = std::make_shared<ConfigurationContent<uint64_t> >(file[property].get<uint64_t>());
                }
                childs[property] = std::make_shared<Configuration>(new_content);
            }
            else {
                childs[property] = std::make_shared<Configuration>(file[property]);
            }
        }
        else {
            childs[property] = std::make_shared<Configuration>();
        }
    }
    LOG(DEBUG) << "Return at " << property;
    return *childs[property];
}

Configuration& Configuration::operator[](const std::string &s) {
    return this->at(s);
}

void Configuration::setArgv(int argc, char* argv[]) {
    this->argv = std::vector<std::string>(argv, argv + argc);
}

std::string Configuration::getArgv(int pos) {
    if (pos > argv.size() || pos < 0) {
        
    }
    return std::string(argv[pos]);
}