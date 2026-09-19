#include <core/base/node/Node.h>

namespace cg::entities::cpp {
    class Namespace : public cg::core::TreeNode {
    public:
        Namespace();
        Namespace(Node* parent);
        
    };

    class Class : public cg::core::TreeNode  {

    };
}