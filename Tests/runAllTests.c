#include "stdlib.h"
#include "testCRC.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(testCRC);
	return UNITY_END();
}
