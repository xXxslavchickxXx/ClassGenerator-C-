#pragma once

#include <core/base/interfaces/IEntity.h>

namespace cg::entities::cpp {
	// Классы заглушки
	class INamespace : public core::IEntity {};
    class IClass : public core::IEntity {};
    class IType : public core::IEntity {};
    class IAlias : public core::IEntity {};
    class IVariable : public core::IEntity {};
}