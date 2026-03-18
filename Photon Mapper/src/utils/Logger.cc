/**
 * @file Logger.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Logger.hh"

#include <iostream>

const std::map<Logger::Level, std::string> Logger::prefix {
    {  Logger::L_DEBUG,   "[DEBUG] "},
    {   Logger::L_INFO,    "[INFO] "},
    {Logger::L_WARNING, "[WARNING] "},
    {  Logger::L_ERROR,   "[ERROR] "}
};

Logger::Level Logger::output_level = Logger::Level::NONE;

void Logger::log(Logger::Level level, const std::string& msg)
{
    if (level >= output_level) {
        std::ostream& stream = (level == L_ERROR) ? std::cerr : std::cout;
        stream << prefix.at(level) << msg << std::endl;
    }
}

void Logger::debug(const std::string& msg) { log(L_DEBUG, msg); }

void Logger::info(const std::string& msg) { log(L_INFO, msg); }

void Logger::warning(const std::string& msg) { log(L_WARNING, msg); }

void Logger::error(const std::string& msg) { log(L_ERROR, msg); }