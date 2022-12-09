#ifndef TESTS_RTK_ROVER_MANAGER_H
#define TESTS_RTK_ROVER_MANAGER_H

#include <AUnit.h>
#include <RTKRoverManager.h>

using namespace aunit;
using namespace RTKRoverManager;

test(processorWriteToLittleFS) 
{
    String testValue = "TestValue";
    const char* kTestValuePath = "/testValue.txt";
    if (LittleFS.exists(kTestValuePath)) LittleFS.remove(kTestValuePath);
    writeFile(LittleFS, kTestValuePath, testValue.c_str());
    delay(100);
    String savedValue = readFile(LittleFS, kTestValuePath);
    assertTrue(savedValue.equals(testValue));
}

test(deleteFileOnLittleFS)
{
    String testValue = "TestValueToDelete";
    const char PATH_TEST_VALUE[] = "/pathTestValueToDelete.txt";
    if (LittleFS.exists(PATH_TEST_VALUE)) 
    {
        if (!LittleFS.remove(PATH_TEST_VALUE))
        {
            assert("Failed to remove LittleFS path.");
        }
    }
    writeFile(LittleFS, PATH_TEST_VALUE, testValue.c_str());
    delay(100);
    String savedValue = readFile(LittleFS, PATH_TEST_VALUE);
    assertTrue(savedValue.equals(testValue));
    LittleFS.remove(PATH_TEST_VALUE);

    assertFalse(LittleFS.exists(PATH_TEST_VALUE));
}

#endif /*** TESTS_RTK_ROVER_MANAGER_H ***/