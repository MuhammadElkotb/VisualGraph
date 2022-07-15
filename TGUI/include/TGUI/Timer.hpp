/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2022 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_TIMER_HPP
#define TGUI_TIMER_HPP

#include <TGUI/Config.hpp>
#include <TGUI/Duration.hpp>
#include <TGUI/Optional.hpp>

#include <functional>
#include <memory>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Executes callbacks after a certain amount of time
    ///
    /// The static create method will create a timer object that you can start and stop.
    /// The static scheduleCallback method is for when the timer only needs to be triggered a single time.
    ///
    /// This timer is not intended to be used when high accuracy is required.
    /// If the callback is e.g. a millisecond too late then this extra time is NOT subtracted from the next scheduled time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Timer : public std::enable_shared_from_this<Timer>
    {
    public:

        typedef std::shared_ptr<Timer> Ptr; // Only provided for potential consistence in user code

        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @param callback  Function without parameters, to be called at every interval when the timer is enabled
        /// @param interval  How often the callback should be called
        /// @param enable    Should the timer be started immediately?
        ///
        /// @return The created timer that can be used to start/stop it later or change the interval.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static std::shared_ptr<Timer> create(const std::function<void()>& callback, Duration interval, bool enable = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @param callback  Function that takes the timer as parameter, to be called at every interval when the timer is enabled
        /// @param interval  How often the callback should be called
        /// @param enable    Should the timer be started immediately?
        ///
        /// @return The created timer that can be used to start/stop it later or change the interval.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static std::shared_ptr<Timer> create(const std::function<void(std::shared_ptr<Timer>)>& callback, Duration interval, bool enable = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Starts a timer
        ///
        /// @param callback  Function to call at every interval when the timer is enabled
        /// @param interval  Time until callback should occur. Don't pass this parameter to call the callback at the next update.
        ///
        /// There is no way to cancel a scheduled call. If you need this functionality then you should create a proper timer
        /// (with the create function) which you then stop when the callback happens.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void scheduleCallback(std::function<void()> callback, Duration interval = Duration());


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the interval at which a timer callback is send
        ///
        /// @param interval  How often the callback should be called
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setInterval(Duration interval);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the interval at which a timer callback is send
        /// @return How often the callback should be called
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Duration getInterval() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Starts or stops a timer
        ///
        /// When enabling a timer that is already enabled, the timer will be restarted.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setEnabled(bool enabled);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the timer is running
        /// @return Is the timer currently enabled?
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isEnabled() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the callback function that should be called by the timer at each interval
        ///
        /// @param callback  Function without parameters, to be called at every interval when the timer is enabled
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setCallback(const std::function<void()>& callback);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the callback function that should be called by the timer at each interval
        ///
        /// @param callback  Function that takes the timer as parameter, to be called at every interval when the timer is enabled
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setCallback(const std::function<void(std::shared_ptr<Timer>)>& callback);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Restarts the timer
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void restart();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Updates the timers and calls their callback functions
        /// @param elapsedTime  Amount of time passed since the last update
        /// @return Was any callback function called?
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static bool updateTime(Duration elapsedTime);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Returns the duration until the next moment a callback function has to be send.
        /// @return Time until next time expires or empty object if there are no timers
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Optional<Duration> getNextScheduledTime();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Removes all active timers, including the ones that TGUI uses internally
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void clearTimers();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor, used by static create and scheduleCallback functions
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Timer() = default;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        static std::vector<std::shared_ptr<Timer>> m_activeTimers;

        bool m_repeats = false;
        bool m_enabled = false;
        Duration m_interval;
        std::function<void()> m_callback;

        Duration m_remainingDuration;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TIMER_HPP
