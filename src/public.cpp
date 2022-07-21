#include "common.hpp"
#include <rpc>
#include <shared_memory>
#include <memory>

size_t pubExists(std::PID client, std::SMID smid) {
	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return std::uregistry::CONNECTION_ERROR;
	char* buffer = (char*)link.f;

	buffer[PAGE_SIZE-1] = 0;
	std::string path(buffer);
	std::sm::unlink(smid);

	auto ret = exists(path);
	return ret;
}

size_t pubCreate(std::PID client, std::SMID smid) {
	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return std::uregistry::CONNECTION_ERROR;
	char* buffer = (char*)link.f;

	buffer[PAGE_SIZE-1] = 0;
	std::string path(buffer);
	std::sm::unlink(smid);

	return create(client, path);
}

static std::Buffer marshalledList(const std::string& path) {
	size_t sz = 0;
	auto l = list(getParts(path));
	for(auto const& x : l)
		sz += x.size() + 1;

	uint8_t* buffer = new uint8_t[sz];
	uint8_t* ptr = buffer;
	for(auto& x : l) {
		memcpy(ptr, x.c_str(), x.size());
		ptr += x.size();
		*(ptr++) = 0;
	}

	return {buffer, sz};
}

size_t pubListSize(std::PID client, std::SMID smid) {
	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return 0;
	char* buffer = (char*)link.f;

	buffer[PAGE_SIZE-1] = 0;
	std::string path(buffer);
	std::sm::unlink(smid);

	return marshalledList(path).size();
}

bool pubList(std::PID client, std::SMID smid) {
	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return false;
	char* buffer = (char*)link.f;

	buffer[PAGE_SIZE-1] = 0;
	std::string path(buffer);

	auto ml = marshalledList(path);
	if(ml.size() > npages * PAGE_SIZE) {
		std::sm::unlink(smid);
		return false;
	}

	memcpy(buffer, ml.get(), ml.size());
	std::sm::unlink(smid);
	return true;
}

void publish() {
	std::exportProcedure((void*)pubExists, 1);
	std::exportProcedure((void*)pubCreate, 1);
	std::exportProcedure((void*)pubListSize, 1);
	std::exportProcedure((void*)pubList, 1);
	std::enableRPC();
	if(!std::publish("registry"))
		std::exit(1);
	std::halt();
}
