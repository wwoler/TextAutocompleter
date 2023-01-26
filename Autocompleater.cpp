#include "Autocompleater.h"

Autocompleater::Autocompleater():
	_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE))
{
	init_dictionary_from_file("words.txt");
}

Autocompleater::~Autocompleater()
{
	CloseHandle(_consoleHandle);
}


auto Autocompleater::init_dictionary_from_file(std::string const& filename) -> void
{
	std::fstream stream(filename, std::ios::in | std::ios::out);
	if (!stream.is_open())
	{
		std::cerr << "Error\n";
		return;
	}

	std::string buf;
	while (stream.good())
	{
		std::getline(stream, buf, '\n');
		_dictionary.insert(buf);
	}

	stream.close();
}

auto Autocompleater::getCoords() -> COORD&
{
	GetConsoleScreenBufferInfo(_consoleHandle, &_cursorInfo);
	return _cursorInfo.dwCursorPosition;
}

auto Autocompleater::setCoords(COORD const& coord) -> void
{
	SetConsoleCursorPosition(_consoleHandle, coord);
}

auto Autocompleater::clearConsoleChar(COORD& currCoord, size_t k) -> void
{
	for (; k > 0; --k)
	{
		std::cout << "\b ";
		currCoord.X -= 1;
		setCoords(currCoord);
	}
}

auto Autocompleater::clearAutocompleateResult(COORD& currCoord, int& clearCount) -> void
{
	COORD clear;
	clear.X = clearCount;
	clear.Y = currCoord.Y + 2;
	setCoords(clear);

	clearConsoleChar(clear, clearCount);

	clearCount = 0;
	setCoords(currCoord);
}


auto Autocompleater::depthSearching(TrieTree::TrieNode const* currNode, char buf[], std::vector<std::string>& vec, int currIndex, int currDepth, int maxDepth) -> void
{
	if (currNode && currNode->isEndOfWord)
	{
		vec.push_back(std::string(buf, currIndex));
	}

	if (currDepth == maxDepth)
		return;

	for (size_t i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (currNode->children[i])
		{
			buf[currIndex] = 'a' + i;

			depthSearching(currNode->children[i], buf, vec, currIndex + 1, currDepth + 1, maxDepth);
		}
	}
}


auto Autocompleater::autocompleateResult(std::string const& part, std::vector<std::string>& rez, int depth) -> void
{
	if (part.empty())
		return;

	char* buff = new char[part.size() + depth + 1]{};
	auto check = _dictionary.checkChunk(part);
	if (check == nullptr) return;

	size_t k;
	for (k = 0; k < part.size(); ++k)
		buff[k] = part[k];

	depthSearching(check, buff, rez, k, 0, depth);
}


auto Autocompleater::input(std::string& inputingResult) -> void
{
	std::string currWord, currPrefix;
	std::vector<std::string> resSrt, autocompSet;

	int letter = 0,  
		lIndex = 0, 
		rIndex = 0,
		clearCharCount = 0,
		wordsCount = 0;

	bool bLIndex = false, bRIndex = false;
	COORD currCoord;
	short startCoord = getCoords().X;
	short lastWordCoord = 0;

	while (letter != 0xD)
	{
		if (_kbhit())
		{
			switch (letter = _getch())
			{
			case 0xD:
				if (!currPrefix.empty())
					resSrt.push_back(currPrefix);
				break;
			case 0x8:
				currCoord = getCoords();
				
				if (currCoord.X > startCoord)
				{
					clearConsoleChar(currCoord);
					if (!currPrefix.empty())
					{
						currPrefix.pop_back();
						currWord.assign(currPrefix);

						if (currPrefix.empty())
							clearAutocompleateResult(getCoords(), clearCharCount);

					}
					if (currCoord.X == lastWordCoord && wordsCount != 0)
					{
						currPrefix.assign(resSrt.back());
						currWord.assign(currPrefix);
						lastWordCoord -= (currPrefix.size() + 1);
						resSrt.pop_back();
						wordsCount -= 1;
					}
				}
				if(getCoords().X == 0 && clearCharCount != 0)
				{
					clearAutocompleateResult(getCoords(), clearCharCount);
					clearCharCount = 0;
				}
				break;
			case 0x20: //space
			{
				resSrt.push_back(currPrefix);
				currPrefix.clear();
				currWord.clear();

				lastWordCoord = getCoords().X;
				wordsCount += 1;

				if (clearCharCount != 0)
					clearAutocompleateResult(getCoords(), clearCharCount);

				std::cout << static_cast<char>(letter);
				break;
			}
			case 0x3C: // <
				if (autocompSet.empty())
					continue;

				currCoord = getCoords();

				clearConsoleChar(currCoord, currPrefix.size());

				bLIndex = true;
				if (bRIndex)
				{
					bRIndex = false;
					lIndex = rIndex - 2;
				}

				lIndex = (lIndex < 0) ? autocompSet.size() - 1 : lIndex;

				currPrefix.assign(autocompSet[lIndex]);
				std::cout << autocompSet[lIndex--];
				break;

			case 0x3E: // >
				if (autocompSet.empty())
					continue;

				currCoord = getCoords();

				clearConsoleChar(currCoord, currPrefix.size());

				bRIndex = true;
				if (bLIndex)
				{
					bLIndex = false;
					rIndex = lIndex + 2;
				}

				rIndex = (rIndex > autocompSet.size() - 1) ? 0 : rIndex;

				currPrefix.assign(autocompSet[rIndex]);
				std::cout << autocompSet[rIndex++];
				break;
			case 0x3F: // ?
			{
				if (clearCharCount != 0)
					clearAutocompleateResult(getCoords(), clearCharCount);
	
				if (autocompSet.size() != 0)
				{
					currCoord = getCoords();
					std::cout << std::endl << std::endl;
					for (size_t i = 0; i < autocompSet.size(); ++i)
					{
						std::cout << "|" << autocompSet[i] << "| ";
						clearCharCount += autocompSet[i].size() + 3;
					}
					setCoords(currCoord);
				}
				break;
			}
			default:
				std::cout << static_cast<char>(letter);
				rIndex = lIndex = 0;
				bLIndex = bRIndex = false;
				currPrefix.push_back(letter);
				currWord.assign(currPrefix);
				break;
			}

			autocompSet.clear();
			autocompleateResult(currWord, autocompSet);

		}
	}

	for (auto const& chunk : resSrt)
	{
		inputingResult.append(chunk).append(" ");
	}
	system("cls");
}