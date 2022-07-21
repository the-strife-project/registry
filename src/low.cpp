#include <common.hpp>
#include <fs> // For simplifyPath

// Low level, regarding the actual registry representation

static Node root;

std::vector<std::string> getParts(const std::string& path) {
	auto ret = std::simplifyPath(path).split('/');
	if(ret.size())
		ret.pop_front();
	return ret;
}

bool loExists(const Parts& parts) {
	Node* cur = &root;
	for(auto const& part : parts) {
		auto it = cur->children.find(part);
		if(it == cur->children.end())
			return false;
		cur = (*it).s;
	}

	return cur;
}

bool loCreate(const Parts& parts, bool recursive) {
	if(!parts.size())
		return false;

	Node* cur = &root;
	for(size_t i=0; i<parts.size()-1; ++i) {
		auto const& part = parts[i];

		auto it = cur->children.find(part);
		bool found = it == cur->children.end();
		if(!found && !recursive)
			return false;
		else if(!found)
			cur->children[part] = new Node;

		cur = cur->children[part];
	}

	// Actual creation
	cur->children[parts[parts.size()-1]] = new Node;

	return true;
}
