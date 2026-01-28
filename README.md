# IPSPatcher

<p align="center">
  <img src="/images/ui.png" alt="UI Program">
</p>

`IPSPatcher` is a small Qt-based tool to apply `.ips` patches to files.

It can be used in two ways:

- As a GUI application (Qt Widgets)
- As a command-line tool (CLI), using the same `PatchFile` core logic

The actual patching logic lives in `IPSPatcherCore` and is reused by both the UI and CLI.

## GUI Usage

The `IPSPatcherUI` window lets you:

- Choose the input file to be patched.
- Choose the `.ips` patch file.
- Choose an output file, or enable the overwrite option to patch in-place.

## CLI Usage

The same executable also supports a simple command-line mode for quick patching.

### Syntax

```bash
IPSPatcherUI.exe <input_file> <patch.ips> <*optional* output_file>
```

### Examples

```bash
IPSPatcherUI.exe game.smc fix_patch.ips game_patched.smc
IPSPatcherUI.exe input.bin patch.ips // Overwrites input.bin with the patched data
```

On Windows, run the executable from an existing console window (cmd or PowerShell) to see the printed messages.

## Core API

The patching logic is implemented in `IPSPatcherCore`:

```cpp
bool CheckFileValidity(std::vector<uint8_t> fileData);
bool CheckIPSValidity(std::vector<uint8_t> ipsData);
bool ApplyIPSPatch(std::vector<uint8_t>& fileData, const std::vector<uint8_t>& ipsData);
bool OutputPatchedFile(const std::string& outPath, const std::vector<uint8_t>& fileData);
bool PatchFile(std::string filePath, std::string ipsPath, std::string outPath);
```

Both the GUI and CLI call `PatchFile` to apply the `.ips` patch and write the patched file.
