cmake_minimum_required(VERSION 3.17)
project(UI_test_version)

set(CMAKE_CXX_STANDARD 14)

add_executable(UI_test_version test.cpp
        headers/Export.h headers/ReturnCode.h headers/ILogger.h headers/IVector.h headers/ISet.h headers/ICompact.h
        impl/ILogger.cpp impl/ILoggerImpl.cpp impl/IVector.cpp impl/IVectorImpl.cpp
        test/vector.cpp test/set.cpp test/compact.cpp)