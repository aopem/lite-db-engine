#ifndef __LITE_DB_ENGINE_HPP__
#define __LITE_DB_ENGINE_HPP__

#include "query/parser.hpp"
#include "query/query_executor.hpp"
#include "utils/result.hpp"

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

        Result<void> Execute(const std::string &sql);
        void RunInteractive();

    private:
        Parser _parser;
        std::shared_ptr<QueryExecutor> _executor = std::make_shared<QueryExecutor>();
    };
};

#endif // __LITE_DB_ENGINE_HPP__