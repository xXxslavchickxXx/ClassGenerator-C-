#pragma once

namespace cg::core {
    class TreeNode;
    class INode {
    public:
        virtual ~INode() = default;
        virtual TreeNode* as_container() { return nullptr; }
    };
}