#include <FileGenerator/FileCreator.h>
#include <iostream>

namespace fs = std::filesystem;

namespace cg::file {
    bool AtomicProjectGenerator::safe_create_dir(const fs::path& path) {
        if (!fs::exists(path)) {
            std::error_code ec;
            if (fs::create_directories(path, ec)) {
                created_directories.push_back(path);
            }
            else if (ec) {
                std::cerr << "[Error] Failed to create dir: " << path << " (" << ec.message() << ")\n";
                return false;
            }
        }
        return true;
    }

    bool AtomicProjectGenerator::safe_create_file(const fs::path& path, const std::string& initial_content) {
        if (!safe_create_dir(path.parent_path())) {
            return false;
        }

        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "[Error] Failed to open file for writing: " << path << '\n';
            return false;
        }

        if (!initial_content.empty()) {
            file << initial_content;
        }

        created_files.push_back(path);
        return true;
    }

    void AtomicProjectGenerator::rollback() noexcept {
        std::cerr << "[Info] Rolling back created files...\n";

        for (const auto& file : created_files) {
            std::error_code ec;
            fs::remove(file, ec);
        }

        for (auto it = created_directories.rbegin(); it != created_directories.rend(); ++it) {
            std::error_code ec;
            fs::remove(*it, ec);
        }

        created_files.clear();
        created_directories.clear();
    }
}