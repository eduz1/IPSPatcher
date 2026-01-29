#include "pch.h"
#include <vector>
#include "../IPSPatcherCore/IPSPatcherCore.h"

// No file I/O helpers needed anymore

TEST(IPSPatcherCore_CheckIPSValidity, AcceptsValidHeaderAndFooter)
{
    // Minimal valid IPS: "PATCH" + EOF marker
    std::vector<uint8_t> ips = { 'P','A','T','C','H','E','O','F' };
    EXPECT_TRUE(CheckIPSValidity(ips));
}

TEST(IPSPatcherCore_CheckIPSValidity, RejectsTooShortOrBadMagic)
{
    std::vector<uint8_t> tooShort = { 'P','A','T','C','H' };
    EXPECT_FALSE(CheckIPSValidity(tooShort));

    std::vector<uint8_t> badMagic = { 'X','Y','Z','C','H','E','O','F' };
    EXPECT_FALSE(CheckIPSValidity(badMagic));
}

TEST(IPSPatcherCore_ApplyIPSPatch, AppliesSimpleNonRlePatch)
{
    // Base file: 4 bytes of zeros
    std::vector<uint8_t> fileData(4, 0x00);

    // IPS: PATCH + one record at offset 1, size 2, bytes {0xAA, 0xBB} + EOF
    std::vector<uint8_t> ips = {
        'P','A','T','C','H',              // header
        0x00, 0x00, 0x01,                 // offset = 1
        0x00, 0x02,                       // size = 2
        0xAA, 0xBB,                       // data
        'E','O','F'                       // EOF
    };

    EXPECT_TRUE(ApplyIPSPatch(fileData, ips));
    ASSERT_EQ(fileData.size(), 4u);
    EXPECT_EQ(fileData, std::vector<uint8_t>({ 0x00, 0xAA, 0xBB, 0x00 }));
}

TEST(IPSPatcherCore_ApplyIPSPatch, AppliesRlePatchAndResizes)
{
    // Base file: 2 bytes of zeros
    std::vector<uint8_t> fileData(2, 0x00);

    // IPS: PATCH + one RLE record at offset 1, rleSize 3, value 0xCC + EOF
    std::vector<uint8_t> ips = {
        'P','A','T','C','H',              // header
        0x00, 0x00, 0x01,                 // offset = 1
        0x00, 0x00,                       // size = 0 => RLE
        0x00, 0x03,                       // rleSize = 3
        0xCC,                             // rleValue
        'E','O','F'                       // EOF
    };

    EXPECT_TRUE(ApplyIPSPatch(fileData, ips));
    ASSERT_EQ(fileData.size(), 4u);      // resized to offset(1) + size(3)
    EXPECT_EQ(fileData, std::vector<uint8_t>({ 0x00, 0xCC, 0xCC, 0xCC }));
}

TEST(IPSPatcherCore_ApplyIPSPatch, FailsOnTruncatedRecordHeader)
{
    // Base file: 4 bytes of zeros
    std::vector<uint8_t> fileData(4, 0x00);

    // IPS: header + start of a record, but not enough bytes (needs 3 offset + 2 size)
    std::vector<uint8_t> ips = {
        'P','A','T','C','H',  // header
        0x00, 0x00            // only 2 bytes of offset, missing the rest
        // truncated here
    };

    EXPECT_FALSE(ApplyIPSPatch(fileData, ips));
}

TEST(IPSPatcherCore_ApplyIPSPatch, FailsOnTruncatedNonRleData)
{
    std::vector<uint8_t> fileData(4, 0x00);

    // IPS: header + full header for a 2-byte record, but provide only 1 byte of data
    std::vector<uint8_t> ips = {
        'P','A','T','C','H',  // header
        0x00, 0x00, 0x01,     // offset = 1
        0x00, 0x02,           // size = 2
        0xAA                  // only 1 byte of data, should be 2
        // truncated here
    };

    EXPECT_FALSE(ApplyIPSPatch(fileData, ips));
}

TEST(IPSPatcherCore_ApplyIPSPatch, FailsOnTruncatedRleHeader)
{
    std::vector<uint8_t> fileData(2, 0x00);

    // IPS: header + RLE record declaration, but no rleSize/rleValue bytes
    std::vector<uint8_t> ips = {
        'P','A','T','C','H',  // header
        0x00, 0x00, 0x01,     // offset = 1
        0x00, 0x00            // size = 0 => RLE, but missing 2 bytes rleSize + 1 byte value
        // truncated here
    };

    EXPECT_FALSE(ApplyIPSPatch(fileData, ips));
}