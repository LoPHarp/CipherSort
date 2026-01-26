# CipherSort CLI

CipherSort is a high-performance C++ console application designed for advanced text data processing, secure file encryption, and automated link management. The project features a decoupled architecture where core logic is separated from the user interface, ensuring efficiency and scalability.

---

## Key Features

### 1. Advanced Data Sorting
The sorting engine is optimized for memory efficiency and provides flexible processing modes:
* **Prefix Sorting:** Groups lines based on the first N characters.
* **Alphabetical Sorting:** Performs indentation-agnostic comparison by ignoring leading whitespace.
* **Hybrid Mode:** Combines prefix grouping with alphabetical sorting within each group.
* **Whole Line Sorting:** Standard processing based on the entire string content.
* **Duplicate Management:** Includes three distinct modes: Keep All, Deduplicate (unique entries only), and Move to End (collects all repeating lines in a separate section).

### 2. File Cryptography
Secure data handling implemented with streaming encryption algorithms:
* **Algorithms:** Full support for RC4 and XOR encryption methods.
* **Integrity Check:** Uses a custom file signature (CPSORT) to verify password correctness and prevent data corruption during decryption.
* **Large File Support:** Utilizes a 64KB buffered stream for processing files of any size with a low memory footprint.

### 3. Smart Link Executor
A dedicated utility for batch processing URLs discovered within text files:
* Scans and parses links (http, www) into an indexed list.
* Allows the user to execute specific ranges of links (e.g., lines 5 to 15) using the default system browser.

### 4. File Actions and Navigation
Integrated utilities for efficient file system interaction:
* **View Content:** Direct console output of file data with support for trimmed or original formatting.
* **System Integration:** Quick shortcuts to reveal files in Windows Explorer or open them with default associated applications.
* **Smart Path Resolution:** Automatically detects processed (sorted) versions of files and offers selection prompts.

---

## Technical Details

* **Language:** C++17
* **Standard Library:** Heavy use of std::filesystem for path management and std::chrono for performance benchmarking.
* **Architecture:** Decoupled Core Logic (Model) and Console Interface (View).
* **Platform:** Windows (utilizing WinAPI and ShellExecute).
* **Build System:** CMake.

---

## Building from Source

To build CipherSort CLI, ensure you have a C++17 compliant compiler and CMake installed.

1. Clone the repository:
```bash
   git clone https://github.com/LoPHarp/CipherSort.git
```
3. Navigate to the src directory and create a build folder:
```bash
   mkdir build && cd build
```
5. Configure and build the project:
```bash
   cmake ..
   cmake --build .
```
---

## Usage Guide

Run the application and follow the interactive menu:
1. **Set File:** Select a target file via manual entry, step-by-step navigation, or Windows file dialog.
2. **Sort File:** Configure sorting methods and duplicate handling.
3. **Cipher File:** Encrypt or decrypt files using a secure key.
4. **Additional Actions:** View content, open folders, or batch-open links.
