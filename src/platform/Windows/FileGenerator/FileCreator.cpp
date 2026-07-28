#include <FileGenerator/FileCreator.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <nfd.hpp>

namespace fs = std::filesystem;

namespace cg::file {
	std::string open_geometry_path_dialog(const nfdu8char_t* defaultPath) {
        NFD::Guard nfdGuard;

        // auto-freeing memory
        NFD::UniquePath outPath;

        // show the dialog
        nfdresult_t result = NFD::PickFolder(outPath, defaultPath);
        if (result == NFD_OKAY) {
            return outPath.get();
        }
        else if (result == NFD_CANCEL) {
            return "";
        }
        else {
            std::cout << "Error: " << NFD::GetError() << std::endl;
            return "";
        }
	}
}