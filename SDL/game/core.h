#ifndef CORE_H
#define CORE_H

#include <stdexcept>
#include <string>
#include <list>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include "vector2f.h"

#include <direct.h>

inline bool fexists(const char *path) {
	std::ifstream file(path);
	return file.good();
}

struct Version {
	Version(int _major = 0, int _minor = 0, int _rev = 0)
		: major(_major), minor(_minor), rev(_rev) { }
	const int major;
	const int minor;
	const int rev;

	std::string toString() const {
		std::stringstream ss("");
		ss << major << "." << minor << "." << rev;
		return ss.str();
	}
	
	bool operator ==(const Version &rhs) {
		return major == rhs.major && minor == rhs.minor && rev == rhs.rev;
	}

	friend std::ostream &operator <<(std::ostream &os, const Version &ver);
};

inline std::ostream &operator <<(std::ostream &os, const Version &ver) {
	return os << ver.toString();
}

class CWD {
	public:
		static const std::string& getCWD() {
			if (obj.path == "") {
				setCWD(internalGetCWD());
			}
			return obj.path;
		}

		static void setCWD(const char *path) {
			obj.path = path;
			// perform some sanitising
			auto end = obj.path.end() - 1;
			bool clear = false;
			while (*end != '\\' && *end != '/') {
				if (*end == '.') {
					clear = true;
				}
				--end;
			}
			if (clear) {
				obj.path.erase(end + 1, obj.path.end());
			}
		}

		static const char *makeDir(const char *file) {
			obj.current = getCWD() + file;
			return obj.current.c_str();
		}

	private:
		CWD() { }
		static CWD obj;
		std::string current;
		std::string path;
		static const char *internalGetCWD() {
			static char path[FILENAME_MAX] = { 0 };
			if (path[0] == 0) {
				if (!_getcwd(path, sizeof(path))) {
					throw std::runtime_error("getCWD failed!");
				}
			}
			return path;
		}
};

class InternalException: public std::exception {
	public:
		InternalException(const char *str)
			: msg(str) { }
		~InternalException() throw() { }
		const char *what() const throw() {
			std::string str(msg);
			str = "warning: " + str;
			return str.c_str();
		}
		
	private:
		const char *msg;
};

#endif
