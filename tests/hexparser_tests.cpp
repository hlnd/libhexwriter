#include "CppUTest/TestHarness.h"

#include <string.h>
#include <stdbool.h>

extern "C"
{
    #include "hexparser.h"

    #include "nrf_error.h"

    extern uint8_t nibble_from_hex_char(char c);
}

TEST_GROUP(hexparser)
{
    hexparser_record record;

    void setup()
    {
        record.type = INVALID_RECORD;
        record.address = 0xFFFF;
    }

    void teardown()
    {
        memset(&record, 0, sizeof record);
    }
};

TEST(hexparser, test_nibble_from_hex_char)
{
    LONGS_EQUAL(0x0, nibble_from_hex_char('0'));
    LONGS_EQUAL(0x1, nibble_from_hex_char('1'));
    LONGS_EQUAL(0x2, nibble_from_hex_char('2'));
    LONGS_EQUAL(0x3, nibble_from_hex_char('3'));
    LONGS_EQUAL(0x4, nibble_from_hex_char('4'));
    LONGS_EQUAL(0x5, nibble_from_hex_char('5'));
    LONGS_EQUAL(0x6, nibble_from_hex_char('6'));
    LONGS_EQUAL(0x7, nibble_from_hex_char('7'));
    LONGS_EQUAL(0x8, nibble_from_hex_char('8'));
    LONGS_EQUAL(0x9, nibble_from_hex_char('9'));
    LONGS_EQUAL(0xA, nibble_from_hex_char('a'));
    LONGS_EQUAL(0xA, nibble_from_hex_char('A'));
    LONGS_EQUAL(0xB, nibble_from_hex_char('b'));
    LONGS_EQUAL(0xB, nibble_from_hex_char('B'));
    LONGS_EQUAL(0xC, nibble_from_hex_char('c'));
    LONGS_EQUAL(0xC, nibble_from_hex_char('C'));
    LONGS_EQUAL(0xD, nibble_from_hex_char('d'));
    LONGS_EQUAL(0xD, nibble_from_hex_char('D'));
    LONGS_EQUAL(0xE, nibble_from_hex_char('e'));
    LONGS_EQUAL(0xE, nibble_from_hex_char('E'));
    LONGS_EQUAL(0xF, nibble_from_hex_char('f'));
    LONGS_EQUAL(0xF, nibble_from_hex_char('F'));
}

TEST(hexparser, test_data_record_parse_succeed)
{
    const char * data_record = ":100110002146017EB7C20001FF5F16002148011988";
    LONGS_EQUAL(NRF_SUCCESS, hexparser_parse_string(data_record, strlen(data_record), &record));
}

TEST(hexparser, test_data_record_identification)
{
    const char * data_record = ":100110002146017EB7C20001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);
    
    LONGS_EQUAL(DATA_RECORD, record.type);
}

TEST(hexparser, test_extended_linear_record_identifcation)
{
    const char * data_record = ":020000040000FA";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(EXTENDED_LINEAR_ADDRESS_RECORD, record.type);
}

TEST(hexparser, test_start_segment_address_record_identification)
{
 
    const char * data_record = ":040000031000419513";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(START_SEGMENT_ADDRESS_RECORD, record.type);
}

TEST(hexparser, test_extended_end_of_file_identification)
{
    const char * data_record = ":00000001FF";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(END_OF_FILE_RECORD, record.type);
}

TEST(hexparser, test_address_parsing)
{
    const char * data_record = ":100110002146017EB7C20001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x0110, record.address);

    data_record = ":1013200073302E6300000000B00200201000002087";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x1320, record.address);
}

TEST(hexparser, test_byte_count_parsing)
{
    const char * data_record = ":100110002146017EB7C20001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x10, record.byte_count);

    data_record = ":020000041000EA";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x02, record.byte_count);
}

TEST(hexparser, test_checksum_parsing)
{
    const char * data_record = ":100110002146017EB7C20001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x88, record.checksum);
}

TEST(hexparser, test_data_parsing)
{
    const char * data_record = ":1010000000400100FFFFFFFFFFFFFFFFFFFFFFFFAB";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x14000, record.data.words[0]);

    data_record = ":0410D000000000001C";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(0x0, record.data.words[0]);

    data_record = ":10010000A246AB4633431847980E0100B80E0100D3";
    hexparser_parse_string(data_record, strlen(data_record), &record);
    LONGS_EQUAL(0x46AB46A2, record.data.words[0]); 
    LONGS_EQUAL(0x47184333, record.data.words[1]); 
    LONGS_EQUAL(0x00010E98, record.data.words[2]); 
    LONGS_EQUAL(0x00010EB8, record.data.words[3]); 
}

TEST(hexparser, test_extended_segment_address_record_parsing)
{
    const char * hexline = ":020000021000EC";

    hexparser_parse_string(hexline, strlen(hexline), &record);

    LONGS_EQUAL(EXTENDED_SEGMENT_ADDRESS_RECORD, record.type);
    LONGS_EQUAL(0x02, record.byte_count);
    LONGS_EQUAL(0x0, record.address);
    LONGS_EQUAL(0x1000, record.data.words[0]);
    LONGS_EQUAL(0xEC, record.checksum);
}

TEST(hexparser, test_set_extended_address_record_parsing)
{
    const char * data_record = ":020000041000EA";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(EXTENDED_LINEAR_ADDRESS_RECORD, record.type);
    LONGS_EQUAL(0x02, record.byte_count);
    LONGS_EQUAL(0x0, record.address);
    LONGS_EQUAL(0x1000, record.data.words[0]);
    LONGS_EQUAL(0xEA, record.checksum);
}

TEST(hexparser, test_is_valid)
{
    const char * data_record = ":020000041000EA";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));

    data_record = ":040000031000419513";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));

    data_record = ":104000000040002095410100D5410100D741010049";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));

    data_record = ":1040B0000000000000000000000000000000000000";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));

    data_record = ":100110002146017EB7C20001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));

    data_record = ":040000032000019543";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(true, hexparser_is_record_valid(&record));
}

TEST(hexparser, test_is_not_valid)
{
    const char * data_record = ":020000041000FA";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(false, hexparser_is_record_valid(&record));

    data_record = ":100110002146017EB7C21001FF5F16002148011988";
    hexparser_parse_string(data_record, strlen(data_record), &record);

    LONGS_EQUAL(false, hexparser_is_record_valid(&record));
}
