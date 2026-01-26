#pragma once
#include <string>
#include <vector>

bool CheckFileValidity(std::vector<uint8_t> fileData);
bool CheckIPSValidity(std::vector<uint8_t> ipsData);
bool ApplyIPSPatch(std::vector<uint8_t>& fileData, const std::vector<uint8_t>& ipsData);
bool OutputPatchedFile(const std::string& outPath, const std::vector<uint8_t>& fileData);
bool PatchFile(std::string filePath, std::string ipsPath, std::string outPath);