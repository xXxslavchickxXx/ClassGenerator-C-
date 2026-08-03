#include <string>
#include <filesystem>
#include <nfd.hpp>
#include <fstream>
#include <vector>

namespace cg::file {
	class AtomicProjectGenerator {
		std::vector<std::filesystem::path> created_files;
		std::vector<std::filesystem::path> created_directories;

	public:
		bool safe_create_dir(const std::filesystem::path& path);
		bool safe_create_file(const std::filesystem::path& path, const std::string& initial_content = "");

		void rollback() noexcept;
	};
}