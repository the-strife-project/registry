#include "common.hpp"
#include <rpc>
#include <shared_memory>

size_t connect(std::PID client, std::SMID smid) {
	bool ret = std::sm::connect(client, smid);
	return ret ? std::uregistry::OK : std::uregistry::CONNECTION_ERROR;
}

size_t exists(std::PID client) {
	uint8_t* data = std::sm::get(client);
	if(!data)
		return std::uregistry::CONNECTION_ERROR;

	data[PAGE_SIZE-1] = 0;
	std::string path((char*)data);

	std::printf("%d asked to read %s\n", client, path.c_str());
	return std::uregistry::OK;
}

void publish() {
	std::exportProcedure((void*)connect, 1);
	std::exportProcedure((void*)exists, 0);
	//std::exportProcedure((void*)pubCreate, 1);
	std::enableRPC();
	if(!std::publish("registry"))
		std::exit(1);
	std::halt();
}
