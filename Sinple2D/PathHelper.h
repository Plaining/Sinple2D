#pragma once
#include "common.h"
#include <string>
namespace Simple2D {
	class DLL_export PathHelper {
	public:
		PathHelper();
		static std::string fullPath(const std::string& filename);
	private:
		static PathHelper pathHelper;
		std::string sAppPath;
		std::string sAssertName;
		std::string sAssertPath;
	};
}