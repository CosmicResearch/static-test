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

#include "catch.hpp"
#include "Listener.h"
#include "Dispatcher.h"
#include "StubListener.h"

TEST_CASE("Subscribe a listener to an event") {

    Dispatcher dispatcher;

    std::shared_ptr<Listener> listener = std::make_shared<StubListener>(&dispatcher);

    SECTION("With a bad event") {
        REQUIRE_THROWS_AS(dispatcher.subscribe(-1, listener), bad_event_exception);
    }
    SECTION("With a valid event") {
        REQUIRE_NOTHROW(dispatcher.subscribe(EVENT_UNDEFINED, listener));
        dispatcher.start();
        REQUIRE(listener.get()->isStarted());
    }

}

TEST_CASE("Dispatch an event") {

    SECTION("With a bad event") {

        Dispatcher dispatcher;
        dispatcher.start();

        REQUIRE_THROWS_AS(dispatcher.dispatch(-1), bad_event_exception);
    }
    SECTION("Without data") {

        Dispatcher dispatcher;
        std::shared_ptr<Listener> listener = std::make_shared<StubListener>(&dispatcher);
        dispatcher.subscribe(EVENT_APOGEE, listener);
        dispatcher.start();

        REQUIRE_NOTHROW(dispatcher.dispatch(EVENT_APOGEE));

        dispatcher.stop();

        REQUIRE(((StubListener*)listener.get())->getCount() == 1);
        REQUIRE(((StubListener*)listener.get())->getLastEvent() == EVENT_APOGEE);
    }
    SECTION("With data") {

        Dispatcher dispatcher;
        std::shared_ptr<Listener> listener = std::make_shared<StubListener>(&dispatcher);
        dispatcher.subscribe(EVENT_APOGEE, listener);
        dispatcher.start();

        Variant v((uint8_t)5);
        REQUIRE_NOTHROW(dispatcher.dispatch(EVENT_APOGEE, v));
        REQUIRE_NOTHROW(dispatcher.dispatch(EVENT_APOGEE, v));

        dispatcher.stop();

        REQUIRE(((StubListener*)listener.get())->getCount() == 2);
        REQUIRE(((StubListener*)listener.get())->getLastEvent() == EVENT_APOGEE);
        REQUIRE(((StubListener*)listener.get())->getLastData().toUInt8() == 5);
    }

}

TEST_CASE("Two starts") {

    Dispatcher dispatcher;

    REQUIRE_NOTHROW(dispatcher.start());
    REQUIRE_THROWS_AS(dispatcher.start(), already_started_exception);

}