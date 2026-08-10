#include <StructFileGen/StructFileManager.h>
#include <iostream>

namespace fs = std::filesystem;

namespace cg::file {

    std::string open_geometry_path_dialog(const nfdu8char_t* defaultPath) {
        NFD::Guard nfdGuard;
        NFD::UniquePath outPath;

        nfdresult_t result = NFD::PickFolder(outPath, defaultPath);
        if (result == NFD_OKAY) {
            return outPath.get();
        }
        if (result == NFD_ERROR) {
            std::cerr << "[NFD Error] " << NFD::GetError() << '\n';
        }
        return "";
    }

    struct_file_manager::struct_file_manager(
        std::string class_name_,
        fs::path parent_path_,
        fs::path abstract_path_,
        DEFINITION_TYPE cpp_definition_,
        DEFINITION_TYPE inline_definition_)
        : class_name(std::move(class_name_))
        , parent_path(std::move(parent_path_))
        , abstract_path(std::move(abstract_path_))
        , cpp_definition(cpp_definition_)
        , inline_definition(inline_definition_)
    {}

    bool struct_file_manager::create_structure() {
        fs::path header_path = parent_path / "include" /
        abstract_path / (class_name + ".h");
        if (!afm.safe_create_file(header_path)) {
            afm.rollback();
            return false;
        }

        auto safe_file_creator = [&](const fs::path& path) {
            if (!afm.safe_create_file(path)) {
                afm.rollback();
                return false;
            }
            return true;
        };

        struct PlatformMapping {
            DEFINITION_TYPE type;
            std::string folder_name;
        };

        const PlatformMapping platforms[] = {
            { DEFINITION_TYPE::WINDOWS, "windows" },
            { DEFINITION_TYPE::LINUX,   "linux" },
            { DEFINITION_TYPE::APPLE,   "apple" }
        };

        if (cpp_definition != DEFINITION_TYPE::NO_DEFINITION) {
            if (has_platform(cpp_definition,
            DEFINITION_TYPE::CROSSPLATOFORM)) {
                fs::path cpp_path = parent_path / "src" /
                abstract_path / (class_name + ".cpp");
                if (!safe_file_creator(cpp_path)) return false;
            }

            for (const auto& [type, folder] : platforms) {
                if (has_platform(cpp_definition, type)) {
                    fs::path cpp_path = parent_path / "src" /
                    "platform" / folder / abstract_path /
                    (class_name + ".cpp");
                    if (!safe_file_creator(cpp_path)) return false;
                }
            }
        }

        if (inline_definition != DEFINITION_TYPE::NO_DEFINITION) {
            if (has_platform(inline_definition,
            DEFINITION_TYPE::CROSSPLATOFORM)) {
                fs::path inline_path = parent_path / "src" /
                    abstract_path / (class_name + ".inl");
                if (!safe_file_creator(inline_path)) return false;
            }

            for (const auto& [type, folder] : platforms) {
                if (has_platform(inline_definition, type)) {
                    fs::path inline_path = parent_path / "src" /
                    "platform" / folder / abstract_path /
                    (class_name + ".inl");
                    if (!safe_file_creator(inline_path)) return false;
                }
            }
        }

        return true;
    }

}