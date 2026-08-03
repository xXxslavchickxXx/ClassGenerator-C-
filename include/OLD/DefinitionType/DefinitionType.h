#pragma once

#include <filesystem>

namespace cg::file {
	/// <summary>
	/// @brief По сути генерацию определения можно
	/// выделить как отдельный флаг, так как определения
	/// может вовсе не быть или быть, но под конкретный запрос
	/// под платформу или кроссплатформенный код
	/// </summary>
	enum class DEFINITION_TYPE {
		NO_DEFINITION = 0,
		CROSSPLATOFORM = 1 << 0,
		WINDOWS = 1 << 1,
		LINUX = 1 << 2,
		APPLE = 1 << 3
	};

	constexpr DEFINITION_TYPE operator|(DEFINITION_TYPE a, DEFINITION_TYPE b) {
		return static_cast<DEFINITION_TYPE>(
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(a) |
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(b)
			);
	}

	constexpr DEFINITION_TYPE& operator|=(DEFINITION_TYPE& a, DEFINITION_TYPE b) {
		a = static_cast<DEFINITION_TYPE>(
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(a) |
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(b)
			);
		return a;
	}

	constexpr bool has_platform(DEFINITION_TYPE mask, DEFINITION_TYPE check) {
		return (static_cast<std::underlying_type_t<DEFINITION_TYPE>>(mask) &
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(check)) != 0;
	}
}