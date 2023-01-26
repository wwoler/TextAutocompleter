#include "TrieTree.h"


TrieTree::TrieTree() :
	_root(getNewNode())
{}

TrieTree::TrieNode::~TrieNode()
{
	for (size_t i = 0u; i < ALPHABET_SIZE; ++i)
		delete children[i];
}

TrieTree::~TrieTree()
{
	if (_root)
	{
		delete _root;
	}
}



auto TrieTree::getNewNode() -> TrieNode*
{
	TrieNode* newNode = new TrieNode;

	newNode->isEndOfWord = false;

	for (size_t i = 0u; i < ALPHABET_SIZE; ++i)
		newNode->children[i] = nullptr;

	return newNode;
}

auto TrieTree::removeInner(TrieNode* CurrNode, std::string const& key, int depth) -> TrieNode*
{
	if (!CurrNode) return nullptr;

	if (depth == key.size())
	{
		if (CurrNode->isEndOfWord)
			CurrNode->isEndOfWord = false;
		if (isEmpty(CurrNode))
		{
			delete CurrNode;
			CurrNode = nullptr;
		}

		return CurrNode;
	}

	int letter
		= static_cast<int>(key[depth]) - static_cast<int>('a');
	CurrNode->children[letter] = removeInner(CurrNode->children[letter], key, depth + 1);

	if (isEmpty(CurrNode) && CurrNode->isEndOfWord == false)
	{
		delete CurrNode;
		CurrNode = nullptr;
	}

	return CurrNode;
}

auto TrieTree::isEmpty(TrieNode* currNode) -> bool
{
	for (size_t i = 0u; i < ALPHABET_SIZE; ++i)
		if (currNode->children[i])
			return false;

	return true;
}


auto TrieTree::insert(std::string& key) -> void
{
	if (key.empty()) return;

	TrieNode* currNode = _root;

	while (!key.empty())
	{
		int letter
			= static_cast<int>(key[0]) - static_cast<int>('a');

		key.erase(0, 1);

		if (letter > 25 || letter < 0) continue;
		
		if (currNode->children[letter] == nullptr)
		{
			TrieNode* newNode(getNewNode());
			currNode->children[letter] = newNode;
		}

		currNode = currNode->children[letter];
	}

	currNode->isEndOfWord = true;
}

auto TrieTree::remove(std::string const& key) -> TrieNode*
{
	if (key.empty()) return nullptr;

	TrieNode* currNode = _root;
	return removeInner(currNode, key);
}

auto TrieTree::search(std::string const& key) -> bool 
{
	TrieNode* currNode = _root;

	for (size_t i = 0u; i < key.size(); ++i)
	{
		int letter
			= static_cast<int>(key[i]) - static_cast<int>('a');

		if (currNode->children[letter] == nullptr) return false;

		currNode = currNode->children[letter];
	}

	return (currNode && currNode->isEndOfWord);
}

auto  TrieTree::checkChunk(std::string const& chunk) -> TrieNode const*
{
	TrieNode* currNode = _root;

	for (size_t i = 0u; i < chunk.size(); ++i)
	{
		int letter
			= static_cast<int>(chunk[i]) - static_cast<int>('a');

		if (letter < 0 || letter > 25 || currNode->children[letter] == nullptr) return nullptr;

		currNode = currNode->children[letter];
	}

	return currNode;
}