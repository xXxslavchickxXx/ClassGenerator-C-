#pragma once

#include <string>
#include <Macros.h>

namespace cg::src {
	class NamedEntity {
		std::string name;

	public:
		NamedEntity(const std::string& name);

		const std::string& get_name() const;
		void set_name(const std::string& new_name);
	};

	/// <summary>
	/// @brief По сути генерацию определения можно
	/// выделить как отдельный флаг, так как определения
	/// может вовсе не быть или быть, но под конкретный запрос
	/// под платформу или кроссплатформенный код
	/// </summary>
	enum class DEFINITION_TYPE : uint16_t {
		NO_DEFINITION = 0,
		CROSSPLATOFORM = 1 << 0,
		WINDOWS = 1 << 1,
		LINUX = 1 << 2,
		APPLE = 1 << 3
	};
	ENABLE_BITMASK_OPERATORS(DEFINITION_TYPE)

	constexpr bool has_platform(DEFINITION_TYPE mask, DEFINITION_TYPE check) {
		return (static_cast<std::underlying_type_t<DEFINITION_TYPE>>(mask) &
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(check)) != 0;
	}
}