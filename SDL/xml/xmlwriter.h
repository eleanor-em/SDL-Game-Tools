#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <fstream>
#include <stack>

class XMLWriter {
	public:
		XMLWriter(const char *fname)
			: file(fname), level(0) { }
	
		bool open() const { return file.is_open(); }

		void close() {
			while (level != 0) {
				closeTag();
			}
			file << "\n";
			file.close();
		}
	
		void openTag(const char *name) {
			tags.push(name);
			printIndents();
			file << "<" << name << ">\n";
			++level;
		}
	
		void closeTag() {
			--level;
			printIndents();
			file << "</" << tags.top() << ">\n";
			tags.pop();
		}
	
		template <class T>
		void printData(const T &data)  {
			printIndents();
			file << data << "\n";
		}
	
		void printIndents() {
			for (unsigned int i = 0; i < level; ++i) {
				file << " ";
			}
		}
		
	private:
		std::ofstream file;	
		unsigned int level;
		std::stack<const char *> tags;
};

#endif