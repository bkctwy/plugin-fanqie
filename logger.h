#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#define ELPP_THREAD_SAFE

#include <easylogging++.h>
#include <string>

INITIALIZE_EASYLOGGINGPP

using namespace std;

class Logger
{
public:
    Logger(const string &log_file_path)
    {
        el::Configurations config;
        config.setToDefault();
        config.setGlobally(el::ConfigurationType::Format, "[%datetime] [%level] %msg");
        el::Loggers::reconfigureLogger("default", config);
    }
    static void info(const string &message)
    {
        LOG(INFO) << message;
    }
    static void error(const string &message)
    {
        LOG(ERROR) << message;
    }
};
#endif // LOGGER_H