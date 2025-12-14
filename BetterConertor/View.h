#pragma once

#include <iostream>
#include <vector>
#include <string>

void ConsoleClear();
void Countdown(int seconds = 3);

void ViewMainMenu();
void ViewsetFolderMenu();
void ViewFileSystemElements(const std::vector<std::string>& items, const std::string& currentPath = "");
void ViewPath(int choise);
void printError(const std::string mes);