#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "FileSorter.h"

void ConsoleClear();
void Countdown(int seconds = 3);

void ViewMainMenu();
void ViewsetFolderMenu();
void ViewFileSystemElements(const std::vector<std::string>& items, const std::string& currentPath = "");
void ViewPath(int choise);
void printError(const std::string mes);
void ViewSortedMenu();
void ViewDuplicateMenu();
void ViewFinalSortedMenu();

void PrintData(const std::vector<std::string>& data);
void PrintData(const std::vector<Group>& groups, bool showHeaders);