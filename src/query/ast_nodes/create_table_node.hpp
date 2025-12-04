#ifndef __CREATE_TABLE_NODE_HPP__
#define __CREATE_TABLE_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"
#include "../data.hpp"
#include "../../stg/table_schema.hpp"

#include <string>
#include <vector>
#include <memory>

namespace litedb
{
    class CreateTableNode : public AstNode, public std::enable_shared_from_this<CreateTableNode>
    {
    public:
        std::string database;
        std::string table;
        TableSchema schema;

        void Accept(const std::shared_ptr<AstNodeVisitor> &visitor) override;
    };
};

#endif // __CREATE_TABLE_NODE_HPP__