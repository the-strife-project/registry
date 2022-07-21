#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <userspace/registry.hpp>

struct Node {
	std::unordered_map<std::string, Node*> children;
};

typedef std::vector<std::string> Parts;
Parts getParts(const std::string& path);
bool loExists(const Parts&);
std::unordered_set<std::string> list(const Parts& parts);
bool loCreate(const Parts& parts, bool recursive=false);

#define UID_SYSTEM 1
size_t exists(const std::string&);
size_t create(std::PID, const std::string&);

void publish();

#endif
