#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "libhexwriter.h"
    #include "nrf_error.h"
}

TEST_GROUP(libhexwriter)
{
};

TEST(libhexwriter, test_write_line_returns_success)
{
    LONGS_EQUAL(NRF_SUCCESS, libhexwriter_write_line("jkl", 3));
}

IGNORE_TEST(libhexwriter, test_write_line_actually_writes)
{

}
