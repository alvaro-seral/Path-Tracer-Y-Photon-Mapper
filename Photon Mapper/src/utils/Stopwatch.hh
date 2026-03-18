/**
 * @file Stopwatch.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include <chrono>
#include <string>

/// @brief Simple stopwatch
class Stopwatch {
private:
    /// @brief Time stamps
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

public:
    /// @brief Start the stopwatch
    void start();

    /// @brief Stop the stopwatch
    void stop();

    /**
     * @brief Return the elapsed time as a string
     *
     * @return Time (m s ms)
     */
    std::string toString() const;
};