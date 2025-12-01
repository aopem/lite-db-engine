#ifndef __NODE_BUILDER_FACTORY_HPP__
#define __NODE_BUILDER_FACTORY_HPP__

#include "node_builder.hpp"
#include "../symbol.hpp"

#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <concepts>
#include <format>

#include <boost/log/trivial.hpp>

namespace litedb
{
    class NodeBuilderFactory
    {
        public:
            template<typename T> requires std::derived_from<T, NodeBuilder>
            void RegisterBuilder(symbol_e type)
            {
                _builder_map[type] = std::make_unique<T>();
            }

            std::unique_ptr<NodeBuilder> CreateBuilder(symbol_e type)
            {
                if (_builder_map.find(type) == _builder_map.end())
                {
                    auto error_msg = std::format("Invalid SQL statement: NodeBuilderFactory does not have AST node builder for type '{}'",
                        symbol_e_map[type]);
                    BOOST_LOG_TRIVIAL(error) << error_msg;
                    throw std::runtime_error(error_msg);
                }

                return std::unique_ptr<NodeBuilder>(_builder_map[type]->Clone());
            }

        private:
            std::unordered_map<symbol_e, std::unique_ptr<NodeBuilder>> _builder_map;
    };
};

#endif // __NODE_BUILDER_FACTORY_HPP__