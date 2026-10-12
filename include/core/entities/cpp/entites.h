#pragma once

#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>
#include <core/entities/cpp/EntitiyMarkers.h>

namespace cg::entities::cpp {
    using Namespace = TreeNodeFabrica<NamedEntity, INamespace>;
    using Class = TreeNodeFabrica<NamedEntity, IClass>;
    using Type = NodeFabrica<ClassHandler, TypeQualificator, TypeConstructor, IType>;
    using Alias = NodeFabrica<NamedEntity, TypeHandler, IAlias>;
    using Variable = NodeFabrica<
        NamedEntity,
        TypeHandler,
        ValueHandler,
        DefinitionEntity,
        OptionalEntity,
        IVariable
    >;
    using Argument = NodeFabrica<
        NamedEntity,
        TypeHandler,
        ValueHandler,
        IArgument
    >;
    using Function = NodeFabrica<
        NamedEntity,
        TypeHandler,
        DefinitionEntity,
        OptionalEntity,
        IFunction
    >;
}