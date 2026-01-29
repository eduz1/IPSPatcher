#include "pch.h"
#include "framework.h"
#include "IPSPatcherCore.h"
#include <fstream>

// In bytes
constexpr size_t MAX_FILE_SIZE = ((1 << 24) - 1) / 8;
constexpr size_t IPS_HEADER_SIZE = 5;

// Only restriction is a file not larger than 2^24-1 bits (2047 Mb)
bool CheckFileValidity(std::vector<uint8_t> fileData)
{
	//todo: check valid size
	/*if (fileData.size() > 0 && fileData.size() <= MAX_FILE_SIZE)
		return true;

	return false;*/

	return true;
}


// Check for valid length and IPS header/footer
bool CheckIPSValidity(std::vector<uint8_t> ipsData)
{
	//todo: check valid max length
	if (ipsData.size() >= 8)
	{
		// Check header/footer
		if (ipsData[0] == 'P' &&
			ipsData[1] == 'A' &&
			ipsData[2] == 'T' &&
			ipsData[3] == 'C' &&
			ipsData[4] == 'H' &&
			ipsData[ipsData.size() - 3] == 'E' &&
			ipsData[ipsData.size() - 2] == 'O' &&
			ipsData[ipsData.size() - 1] == 'F')
		{
			return true;
		}
	}

	return false;
}


// Apply the IPS patch to the file data
bool ApplyIPSPatch(std::vector<uint8_t>& fileData, const std::vector<uint8_t>& ipsData)
{
	size_t i = IPS_HEADER_SIZE, ipsSize = ipsData.size();

	// Process each patch record
	while (i + 3 <= ipsSize) {

		// Check for EOF marker
		if (ipsData[i]     == 'E' &&
			ipsData[i + 1] == 'O' &&
			ipsData[i + 2] == 'F')
		{
			// Reached end of patch records
			return true;
		}

		// offset is 3 bytes + 2 bytes for size
		if (ipsSize < i + 5)
			return false; //Unexpected end of patch file

		uint32_t offset = (ipsData[i++] << 16) | (ipsData[i++] << 8) | ipsData[i++];
		uint16_t size = (ipsData[i++] << 8) | ipsData[i++];

		if (size == 0) { // RLE
			// RLE size is 2 bytes + 1 byte value
			if (ipsSize < i + 3)
				return false; //Unexpected end of patch file

			uint16_t rleSize = (ipsData[i++] << 8) | ipsData[i++];
			uint8_t rleValue = ipsData[i++];

			// If the RLE size is larger than the remaining file size, we append it into the file
			if (fileData.size() < offset + rleSize)
				fileData.resize(offset + rleSize);

			// Write 'val' rle times from offset
			std::fill_n(fileData.begin() + offset, rleSize, rleValue);
		}
		else { // Non-RLE
			// size bytes of data
			if (ipsSize < i + size)
				return false; //Unexpected end of patch file

			// If the RLE size is larger than the remaining file size, we append it into the file
			if (fileData.size() < offset + size)
				fileData.resize(offset + size);

			// Write the data from IPS to file
			std::copy_n(ipsData.begin() + i, size, fileData.begin() + offset);
			i += size;
		}
	}

	// Early exit?
	return false;
}


// Output the patched file to disk
bool OutputPatchedFile(const std::string& outPath, const std::vector<uint8_t>& fileData)
{
	// Create the output stream
	std::ofstream out(outPath, std::ios::out | std::ios::binary);

	if (out.is_open()) {
		// Serialize the file data and output it
		size_t fileSize = fileData.size();
		out.write(reinterpret_cast<const char*>(fileData.data()), fileSize);

		// Check for write errors
		if (out.fail()) {
			out.close();
			return false;
		}

		out.close();
	}
	else
		return false;

	return true;
}


bool PatchFile(std::string filePath, std::string ipsPath, std::string outPath)
{
	std::ifstream inFile(filePath, std::ios::binary);
	std::ifstream inIPS(ipsPath, std::ios::binary);
	
	if (!inFile.is_open() || !inIPS.is_open())
		return false;

	// Copy both .ips and the file into vectors for handling
	std::vector<uint8_t> fileData{ std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };
	std::vector<uint8_t> ipsData{ std::istreambuf_iterator<char>(inIPS), std::istreambuf_iterator<char>() };

	// Check for validity for both files
	if (!CheckFileValidity(fileData) || !CheckIPSValidity(ipsData))
		return false;

	if (!ApplyIPSPatch(fileData, ipsData))
		return false;

	if (!OutputPatchedFile(outPath, fileData))
		return false;

	return true;
}
