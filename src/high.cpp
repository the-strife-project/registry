#include <common.hpp>
#include <syscalls>
#include <cstdio>

// High level, regarding permissions

static size_t getUIDof(std::PID pid) {
	auto info = std::info(pid);
	return info.uid;
}

static std::PID users = 0;
static std::PID resolveUsers() {
	if(!users)
		users = std::resolve("users");
	return users;
}

static bool canAccess(std::PID pid, const Parts& parts) {
	size_t who = getUIDof(pid);

	if(who == UID_SYSTEM || who == resolveUsers())
		return true;

	if(!parts.size())
		return false;

	// Can you modify it?
	auto check = parts;
	check.pop_back();
	check.push_back("SUPER");
	check.push_back(std::uToStr(who));
	if(!loExists(check))
		return false;

	// Is it about SUPER?
	bool aboutSuper = false;
	if(parts.size() > 3)
		if(parts[2] == "SUPER")
			aboutSuper = true;

	if(aboutSuper)
		return true; // All good

	// No, it's a permission. Do YOU have it?
	std::string perm = parts[parts.size()-1];
	check.clear();
	check.push_back("u");
	check.push_back(std::uToStr(who));
	check.push_back(perm);
	return loExists(check);
}

size_t exists(const std::string& path) {
	return loExists(getParts(path))
		? std::uregistry::OK
		:std::uregistry::NOT_FOUND;
}

size_t create(std::PID pid, const std::string& path) {
	auto parts = getParts(path);

	if(!canAccess(pid, parts))
		return std::uregistry::NOT_ALLOWED;

	// If it has premissions to write here, it's safe to say that it's secure
	//   to do it recursively
	loCreate(parts, true);
	return std::uregistry::OK;
}
