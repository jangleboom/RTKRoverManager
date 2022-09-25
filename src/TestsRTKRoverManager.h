#ifndef TESTS_RTK_ROVER_MANAGER_H
#define TESTS_RTK_ROVER_MANAGER_H

#include <AUnit.h>
#include <RTKRoverManager.h>

using namespace aunit;
using namespace RTKRoverManager;

test(processorWriteAndReadSpiffs) 
{
    String testValue = "TestValue";
    const char* PATH_TEST_VALUE = "/pathTestValue";
    if (SPIFFS.exists(PATH_TEST_VALUE)) SPIFFS.remove(PATH_TEST_VALUE);
    writeFile(SPIFFS, PATH_TEST_VALUE, testValue.c_str());
    delay(100);
    String savedValue = readFile(SPIFFS, PATH_TEST_VALUE);
    assertTrue(savedValue.equals(testValue));
}

#endif /*** TESTS_RTK_ROVER_MANAGER_H ***/