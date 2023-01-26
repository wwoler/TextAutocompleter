#pragma once

#include "TrieTree.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>
#include <queue>

class Autocompleater
{
private:
	TrieTree                   _dictionary;
	HANDLE                     _consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO _cursorInfo;

	auto init_dictionary_from_file(std::string const& filename) -> void;

	auto clearConsoleChar(COORD& currCoord, size_t k = 1) -> void;

	auto clearAutocompleateResult(COORD& currCoord, int& clearCount) -> void;

	auto getCoords() -> COORD&;

	auto setCoords(COORD const& coord) -> void;

	auto depthSearching(TrieTree::TrieNode const* currNode, char buf[], std::vector<std::string>& vec, int currIndex, int currDepth, int maxDepth) -> void;

	auto autocompleateResult(std::string const& part, std::vector<std::string>& rez, int depth = 10) -> void;

public:

	Autocompleater();

	~Autocompleater();

	auto input(std::string& inputingResult) -> void;

};