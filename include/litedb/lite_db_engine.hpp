#ifndef __LITE_DB_ENGINE_HPP__
#define __LITE_DB_ENGINE_HPP__

#include "query/parser.hpp"
#include "query/query_executor.hpp"

#include <istream>
#include <ostream>
#include <string>

namespace litedb
{
    class LiteDbEngine
    {
    public:
        LiteDbEngine() = default;
        ~LiteDbEngine() = default;

        void Run(std::istream &input_stream, std::ostream &output_stream);
        void Execute(const std::string &sql);

    private:
        std::unique_ptr<Parser> _parser = std::make_unique<Parser>();
        std::shared_ptr<QueryExecutor> _executor = std::make_shared<QueryExecutor>();
    };
};

#endif // __LITE_DB_ENGINE_HPP__