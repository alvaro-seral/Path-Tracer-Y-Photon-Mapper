/**
 * @file Logger.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include <map>
#include <string>

/// @brief Simple logging utility
class Logger {
public:
    /// @brief Logging levels
    enum Level { L_DEBUG, L_INFO, L_WARNING, L_ERROR, NONE };

    /// @brief Output level
    static Level output_level;

    /**
     * @brief Log a message
     *
     * @param level Message level
     * @param msg Message
     */
    static void log(Level level, const std::string& msg);

    /**
     * @brief Log a debug message
     *
     * @param msg Message
     */
    static void debug(const std::string& msg);

    /**
     * @brief Log an info message
     *
     * @param msg Message
     */
    static void info(const std::string& msg);

    /**
     * @brief Log a warning message
     *
     * @param msg Message
     */
    static void warning(const std::string& msg);

    /**
     * @brief Log an error message
     *
     * @param msg Message
     */
    static void error(const std::string& msg);

private:
    /// @brief Logging prefixes
    const static std::map<Logger::Level, std::string> prefix;
};
