#pragma once

namespace cg::core {
    class TreeNode;
    class INode {
    public:
        virtual ~INode() = 0;
        virtual TreeNode* as_container() { return nullptr; }
    };
}