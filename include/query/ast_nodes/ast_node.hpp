#ifndef __AST_NODE_HPP__
#define __AST_NODE_HPP__

namespace engine::query
{
    class AstNode
    {
        public:
            AstNode() = default;
            virtual ~AstNode() = default;
    };
};

#endif // __AST_NODE_HPP__