#ifndef __NODE_BUILDER_FACTORY_HPP__
#define __NODE_BUILDER_FACTORY_HPP__

#include "node_builder.hpp"
#include "../symbol.hpp"

#include <memory>
#include <unordered_map>
#include <typeinfo>

#include <boost/log/trivial.hpp>

namespace litedb
{
    class NodeBuilderFactory
    {
        public:
            template<typename T>
            void RegisterBuilder(symbol_e type)
            {
                // if not a type derived from NodeBuilder, cannot register
                if (!std::is_base_of<NodeBuilder, T>::value)
                {
                    auto type_name = std::string(typeid(T).name());
                    auto error_msg = "NodeBuilderFactory cannot register type '" + type_name + "', which is not derived from NodeBuilder";
                    BOOST_LOG_TRIVIAL(error) << error_msg;
                    return;
                }

                _builder_map[type] = std::make_unique<T>();
            }

            std::unique_ptr<NodeBuilder> CreateBuilder(symbol_e type)
            {
                if (_builder_map.find(type) == _builder_map.end())
                {
                    auto error_msg = "NodeBuilderFactory does not have AST node builder for type '" + symbol_e_map[type] + "'";
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