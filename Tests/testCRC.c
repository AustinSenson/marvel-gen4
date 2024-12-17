#include "testCRC.h"
#include "string.h"
#include "unity.h"

#include "crc.h"

void testCRC(void)
{
	char *string = "some random string to test";
	TEST_ASSERT_EQUAL_HEX(calculateCRC(string, strlen(string)), 0xE3DA96BA);
}
