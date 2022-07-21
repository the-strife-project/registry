#include "common.hpp"
#include <rpc>
#include <shared_memory>

size_t pubExists(std::PID client, std::SMID smid) {
	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return std::uregistry::CONNECTION_ERROR;
	char* buffer = (char*)link.f;

	buffer[PAGE_SIZE-1] = 0;
	std::string path(buffer);

	auto ret = exists(path);
	std::sm::unlink(smid);
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

	auto ret = create(client, path);
	std::sm::unlink(smid);
	return ret;
}

void publish() {
	std::exportProcedure((void*)pubExists, 1);
	std::exportProcedure((void*)pubCreate, 1);
	std::enableRPC();
	if(!std::publish("registry"))
		std::exit(1);
	std::halt();
}
