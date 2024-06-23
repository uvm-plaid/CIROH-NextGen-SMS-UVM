/**
 * test/main.cpp
 * 
 * File containing main testing code for module functionality/
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include <Arduino.h>
#include <unity.h>

#include "checksum.h"


void test_nmea_checksum(void) {
    char testString[] = "TD \"Hello World!\"";
    size_t length = 17;
    uint8_t checksum = checksum::nmeaChecksum(testString, length);

    uint8_t expectedChecksum = 0x31;
    TEST_ASSERT_EQUAL(expectedChecksum, checksum);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_nmea_checksum);
    UNITY_END();
}
