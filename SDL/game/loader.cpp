#include "loader.h"
#include <cstring>
#include <fstream>
#include <cstdio>

Loader::Loader(const char *filename, Version version, unsigned int _key)
	: doc(true, tinyxml2::COLLAPSE_WHITESPACE), key(_key) {
	decrypt(filename);
	if (doc.LoadFile(TempPath) != tinyxml2::XML_SUCCESS) {
		throw std::runtime_error("could not open save file");
	}
	std::remove(TempPath);
	root = doc.FirstChild();
	if (root == nullptr) {
		throw std::runtime_error("corrupted save file");
	}
	auto element = root->FirstChildElement("Version");
	if (element == nullptr) {
		throw std::runtime_error("save file version missing");
	}
	std::string ver = version.toString();
	if (std::strcmp(ver.c_str(), element->GetText()) != 0) {
		throw InternalException("save file version incorrect");
	}
	blob = root->FirstChildElement("Blob");
	if (blob != nullptr) {
		data = blob->FirstChildElement("data");
	}
}

void Loader::decrypt(const char *filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	std::ofstream out(TempPath, std::ios::out | std::ios::binary);
	char bytes[sizeof(int)], *bytesOut;
	while (in.read(bytes, sizeof(int))) {
		// reinterpret bytes; here we don't handle endianness
		int chunk = *(int *)bytes;
		chunk ^= key;
		bytesOut = (char *)&chunk;
		out.write(bytesOut, sizeof(int));
	}
	in.close();
	out.close();
}