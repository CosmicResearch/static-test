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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Configuration {

public:

    Configuration();

    explicit Configuration(std::string file);

    explicit Configuration(json& json_file);

    Configuration(const Configuration& other);

    void parse(std::string file);

    void setArgv(int argc, char* argv[]);

    std::string getArgv(int pos); 

    Configuration& at(std::string property);

    Configuration& operator[](const std::string &property);

    template <typename T>
    void set(T s) {
        if (this->content == nullptr) {
            this->content = std::make_shared<ConfigurationContent<T> >(s);
        }
        else {
            this->content->set<T>(s);
        }
    }

    template <typename T>
    void operator=(T& s) {
        this->set(s);
    }

    template <typename T>
    const T& get() const {
        return this->content->get<T>();
    }

    class ConfigurationContentBase {

    public:
        
        virtual ~ConfigurationContentBase() {}
        
        template<typename T>
        const T& get() const {
            return dynamic_cast<const ConfigurationContent<T>&>(*this).get();
        }

        template<typename T, typename U>
        //template<typename T>
        void set(const U& content) {
            std::cout << "ConfigurationContentBase setting to " << content << std::endl;
            return dynamic_cast<ConfigurationContent<T>&>(*this).set(content);
        }

        template<typename T, typename U>
        void operator=(const U& content) {
            set(content);
        }

    };

    template <typename T>
    class ConfigurationContent : public ConfigurationContentBase {

        public:
            
            ConfigurationContent(const T& content) : content(content) {

            }
            
            const T& get() const {
                return content;
            }
            
            void set(const T& content) {
                this->content = content;
            }    

        private:

            T content;

    };

    Configuration(std::shared_ptr<ConfigurationContentBase> content);

private:

    json file;

    std::vector<std::string> argv;

    std::unordered_map<std::string, std::shared_ptr<Configuration> > childs;

    std::shared_ptr<ConfigurationContentBase> content;

    Configuration(json& json_file, std::shared_ptr<ConfigurationContentBase> content);

};