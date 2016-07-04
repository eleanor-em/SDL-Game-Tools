#include "saver.h"
#include <fstream>
#include <cstdio>

void Saver::close() {
	if (writer.open()) {
		writer.close();
		// encrypt
		std::ifstream in(file.c_str(), std::ios::in | std::ios::binary);
		std::ofstream out(TempPath, std::ios::out | std::ios::binary);
		char bytes[sizeof(int)], *bytesOut;
		while (in.read(bytes, sizeof(int))) {
			// reinterpret bytes; here we don't handle endianness
			int data = *(int *)bytes;
			data ^= key;
			bytesOut = (char *)&data;
			out.write(bytesOut, sizeof(int));
		}
		in.close();
		out.close();
		std::remove(file.c_str());
		std::rename(TempPath, file.c_str());
	}
}