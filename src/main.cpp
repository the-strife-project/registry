#include "common.hpp"

Node root;

static bool split(std::string path, std::vector<std::string>& parts) {
	if(path.size() < 1)
		return false;
	path.pop_front();
	path.push_back('/');

	auto it = path.begin();
	++it;

	// State
	std::string last;
	bool quote = false;

	while(it != path.end()) {
		if(*it == '\'')
			quote = !quote;
		else if(*it == '/' && !quote)
			parts.push_back(last);
		else
			last.push_back(*it);

		++it;
	}

	return !quote;
}

size_t find(const std::string& path, Node*& node) {
	// State
	std::vector<std::string> parts;
	if(!split(path, parts))
		return std::uregistry::BADLY_FORMED;

	Node* cur = &root;
	for(auto const& part : parts) {
		for(Node* x : cur->children) {
			if(x->name == part)
				cur = x;
			else
				return std::uregistry::NOT_FOUND;
		}
	}

	node = cur;
	return std::uregistry::OK;
}

extern "C" void _start() {
	publish();
}
