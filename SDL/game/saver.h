#ifndef SAVER_H
#define SAVER_H
#include "../xml/xmlwriter.h"
#include "core.h"

class Saver {
	public:
		Saver(const char *filename, Version version, unsigned int _key = 0x19971997)
			: writer(filename), key(_key), file(filename) {
			writer.openTag("Root");
			writer.openTag("Version");
			writer.printData(version);
			writer.closeTag();
		}

		~Saver() { close(); }

		void close();

		void openBlob() {
			if (writer.open()) {
				writer.openTag("Blob");
			} else {
				throw InternalException("saving to a closed file!");
			}
		}

		template <typename T>
		void putDatum(T data) {
			if (writer.open()) {
				writer.openTag("data"); writer.printData(data); writer.closeTag();
			}
			else {
				throw InternalException("saving to a closed file!");
			}
		}

		void closeBlob() {
			if (writer.open()) {
				writer.closeTag();
			}
			else {
				throw InternalException("saving to a closed file!");
			}
		}

	private:
		const char *TempPath = "temp";
		XMLWriter writer;
		const unsigned int key;
		const std::string file;
};

#endif