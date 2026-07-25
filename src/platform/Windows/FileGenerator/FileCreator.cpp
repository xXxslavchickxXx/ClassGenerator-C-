#include <FileGenerator/FileCreator.h>

#include <Windows.h>

namespace fs = std::filesystem;

namespace cg::file {
	std::string open_geometry_path_dialog(const std::string& win_name) {
		char filename[MAX_PATH] = "";

		OPENFILENAMEA ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;

		ofn.lpstrFilter = "CSV Propeller Geometry (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrTitle = win_name.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn)) {
			return std::string(ofn.lpstrFile);
		}
		return "";
	}
}