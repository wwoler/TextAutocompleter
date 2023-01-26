#pragma once
#include <string>

#define ALPHABET_SIZE  26


class TrieTree
{
public:
	struct TrieNode
	{
		TrieNode* children[ALPHABET_SIZE];
		bool isEndOfWord;

		~TrieNode();
	};

private:
	TrieNode* _root;

	auto getNewNode() -> TrieNode*;
	auto removeInner(TrieNode* CurrNode, std::string const& key, int depth = 0) -> TrieNode*;
	auto isEmpty(TrieNode* currNode) -> bool;

public:
	TrieTree();
	~TrieTree();

	auto insert(std::string& key) -> void;
	auto remove(std::string const& key) -> TrieNode*;
	auto search(std::string const& key) -> bool;
	auto checkChunk(std::string const& chunk) -> TrieNode const*;
};