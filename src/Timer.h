/**
 *        d8888 888                .d8888b.
 *       d88888 888               d88P  Y88b
 *      d88P888 888               888    888
 *     d88P 888 88888b.   .d88b.  888        88888b.d88b.  88888b.
 *    d88P  888 888 "88b d8P  Y8b 888        888 "888 "88b 888 "88b
 *   d88P   888 888  888 88888888 888    888 888  888  888 888  888
 *  d8888888888 888 d88P Y8b.     Y88b  d88P 888  888  888 888 d88P
 * d88P     888 88888P"   "Y8888   "Y8888P"  888  888  888 88888P"
 *                                                         888
 * A simple file comparison tool                           888
 *                                                         888
 * Copyright (c) 2025, Abe Mishler
 * Licensed under the Universal Permissive License v 1.0
 * as shown at https://oss.oracle.com/licenses/upl/.
 */

#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>

class Timer
{
  public:
    Timer() = default;

    void start()
    {
        m_start_time = std::chrono::steady_clock::now();
        m_running = true;
    }

    void stop()
    {
        if (m_running) {
            m_end_time = std::chrono::steady_clock::now();
            m_running = false;
        }
    }

    void printElapsed(const std::string& label = "Elapsed time") const
    {
        if (m_running) {
            std::cout << label << ": timer is still running. Call stop() first.\n";
            return;
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end_time - m_start_time);
        auto ms = duration.count() % 1000;
        auto s = (duration.count() / 1000) % 60;
        auto m = (duration.count() / (1000 * 60)) % 60;
        auto h = (duration.count() / (1000 * 60 * 60));
        std::cout << label << ": ";
        if (h > 0)
            std::cout << h << "h ";
        if (m > 0 || h > 0)
            std::cout << m << "m ";
        if (s > 0 || m > 0 || h > 0)
            std::cout << s << "s ";
        std::cout << ms << "ms\n";
    }

  private:
    std::chrono::steady_clock::time_point m_start_time;
    std::chrono::steady_clock::time_point m_end_time;
    bool m_running = false;
};
