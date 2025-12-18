# FileMaster CLI

FileMaster CLI is a C++ console application designed for efficient file management on Windows. It provides tools for sorting large datasets, encrypting files for security, and batch-executing URLs via the default browser.

The project demonstrates the use of C++17 standards, file stream manipulation, and Windows API integration.

## Key Features

### 1. Advanced File Sorting
Offers flexible sorting algorithms for text data:
* **Sort by Prefix:** Sort lines based on the first 5 or N characters.
* **Filter by Keyword:** Extract lines containing a specific substring.
* **Standard Sort:** Alphabetical ordering.
* **Duplicate Management:** Automatically detects lines repeating 2 or more times. Users can choose to deduplicate them or move them to a separate `[DUPLICATES]` section at the end of the file.

### 2. XOR Encryption
Provides a lightweight encryption mechanism for text files:
* Implements XOR encryption using a custom key.
* Includes a file signature check (`CHECK_PASS_OK`) to prevent data corruption during decryption attempts with incorrect passwords.
* Automatically handles file renaming (e.g., `data.txt` -> `Crypted_data.txt`).

### 3. Link Executor
A tool for batch processing URLs found within text files:
* Scans the selected file for valid URLs (`http`, `www`).
* Parses the file content and presents an indexed list.
* Allows the user to execute a specific range of links using the default system browser.

### 4. File Actions & Navigation
Integrated file system utilities:
* **View Content:** Display file contents directly in the console.
* **Smart Selection:** Automatically detects if a sorted version of the file exists and prompts the user to choose between the original or the processed version.
* **System Integration:** Options to reveal the file in Windows Explorer or open it with the default associated application.

## Technical Details

* **Language:** C++
* **Standard:** C++17 (std::filesystem implementation)
* **Platform:** Windows (WinAPI, ShellExecute)
* **Environment:** Visual Studio

## Installation

1. Clone the repository.
2. Open the solution file (`.sln`) in Visual Studio.
3. Ensure the C++ Language Standard is set to **ISO C++17 Standard** or later in Project Properties.
4. Build the solution in **Release** or **Debug** mode.

## Usage Guide

Run the application and follow the on-screen menu:
1. **Set File:** Select a target file via the file dialog or manual path entry.
2. **Sort File:** Access sorting and deduplication tools.
3. **Cipher File:** Encrypt or decrypt the selected file.
4. **View Content:** Print file contents to the terminal.
5. **Open File Folder:** Reveal the file in Explorer.
6. **Open File:** Launch the file.
7. **Open Links:** Batch open URLs found in the file.