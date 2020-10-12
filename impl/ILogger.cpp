#include "../headers/ILogger.h"
#include "ILoggerImpl.cpp"

ILogger::~ILogger() {}

ILogger* ILogger::createLogger(void* client) {
    return LoggerImpl::getLogger(client);
}
