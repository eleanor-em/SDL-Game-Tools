#include "tinyxml2.h"
#include "xmlwriter.h"
#include "xmlreader.h"
#include <iostream>

int main() {
	XMLWriter writer("test.xml");
	writer.openTag("base");
	writer.openTag("inside");
	writer.printData(1);
	writer.closeTag();
	writer.openTag("inside2");
	writer.printData(2);
	writer.closeTag();
	writer.closeTag();
	writer.openTag("second");
	writer.printData("hello");
	writer.close();
	
	tinyxml2::XMLDocument doc(true, tinyxml2::COLLAPSE_WHITESPACE);
	doc.LoadFile("test.xml");
	auto p = doc.FirstChildElement("base")->FirstChildElement("inside");
	if (p) {
		std::cout << p->GetText() << "\n";
	}
	p = p->NextSiblingElement();
	if (p) {
		std::cout << p->GetText() << "\n";
	}
	p = p->Parent()->Parent()->FirstChildElement("second");
	if (p) {
		std::cout << p->GetText() << "\n";
	}
	system("Pause");
	return 0;
}