#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <unordered_set>
#include <userspace/registry.hpp>
#include <cstdio>

struct Node {
	std::string name;
	std::unordered_set<Node*> children;
};

extern Node root;

void publish();

size_t find(const std::string& path, Node*& node);

#endif
