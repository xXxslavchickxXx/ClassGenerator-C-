#include <string>
#include <filesystem>
#include <nfd.hpp>
#include <fstream>

#include <FileGenerator/FileCreator.h>
#include <DefinitionType/DefinitionType.h>

namespace cg::file {
	// Функция для пользовательского пути
	std::string open_geometry_path_dialog(const nfdu8char_t* defaultPath = "");

	/// <summary>
	/// @brief Класс который отвечает за логику создания файла
	/// и обработки вытекающих конфликтов
	/// 
	/// @cpp_definition Параметр, который отвечает за создание
	/// реализации в еденице трансляции
	/// 
	/// @inline_definition Параметр, который отвечает за создание
	/// реализации шаблонных сущностей класса
	/// </summary>
	class struct_file_manager {
	private:
		DEFINITION_TYPE cpp_definition;
		DEFINITION_TYPE inline_definition;

		std::filesystem::path parent_path;
		std::filesystem::path abstract_path;
		std::string class_name;

		// atomic file manager
		AtomicProjectGenerator afm;

	public:
		struct_file_manager(
			std::string class_name_,
			std::filesystem::path parent_path_,
			std::filesystem::path abstract_path_,
			DEFINITION_TYPE cpp_definition_ = DEFINITION_TYPE::NO_DEFINITION,
			DEFINITION_TYPE inline_definition_ = DEFINITION_TYPE::NO_DEFINITION);

		bool create_structure();
	};
}