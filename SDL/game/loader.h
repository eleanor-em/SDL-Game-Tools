#ifndef LOADER_H
#define LOADER_H
#include "core.h"
#include "../xml/tinyxml2.h"

class Loader {
	public:
		Loader(const char *filename, Version version, unsigned int _key = 0x19971997);

		bool nextBlob() {
			if (blob == nullptr) {
				return false;
			}
			blob = blob->NextSiblingElement("Blob");
			if (blob != nullptr) {
				data = blob->FirstChildElement("data");
				return true;
			}
			return false;
		}
		
		bool hasData() const {
			return data != nullptr;
		}

		bool nextDatum() {
			if (!hasData()) {
				return false;
			}
			data = data->NextSiblingElement();
			return data != nullptr;
		}

		bool getIntValue(int &in) const {
			if (!hasData()) { return false; }
			return data->QueryIntText(&in) == tinyxml2::XML_NO_ERROR;
		}

		bool getFloatValue(float &in) const {
			if (!hasData()) { return false; }
			return data->QueryFloatText(&in) == tinyxml2::XML_NO_ERROR;
		}
		
		const char *getStringValue() const {
			if (!hasData()) { return nullptr; }
			return data->GetText();
		}

	private:
		const char *TempPath = "temp";
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLNode *root;
		tinyxml2::XMLElement *blob;
		tinyxml2::XMLElement *data;
		unsigned int key;

		void decrypt(const char *filename);
};

#endif