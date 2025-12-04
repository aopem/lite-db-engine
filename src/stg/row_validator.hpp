#ifndef __ROW_VALIDATOR_HPP__
#define __ROW_VALIDATOR_HPP__

#include "stg/table_schema.hpp"
#include "query/token.hpp"
#include "query/data.hpp"
#include "query/symbol.hpp"

#include <vector>
#include <string>
#include <string_view>
#include <memory>

namespace litedb
{
    struct ValidationResult
    {
        bool success = true;
        std::string error_message;

        static ValidationResult Ok();
        static ValidationResult Error(std::string_view message);
    };

    class RowValidator
    {
    public:
        RowValidator(TableSchema &schema) : _schema(schema) {}

        ValidationResult Validate(
            const std::vector<std::string> &columns,
            const std::vector<std::shared_ptr<Token>> &values);

    private:
        TableSchema _schema;

        ValidationResult ValidateColumnCount(
            const std::vector<std::string> &columns,
            const std::vector<std::shared_ptr<Token>> &values);
        ValidationResult ValidateColumnExists(std::string_view column_name);
        ValidationResult ValidateColumnType(
            std::string_view column_name,
            const data_type_t &expected_type,
            const Token &value);
        ValidationResult ValidateColumnLength(
            std::string_view column_name,
            const data_type_t &expected_type,
            const Token &value);
        ValidationResult ValidateColumn(
            const std::string &column_name,
            const Token &value);
        bool ValidTypeForColumn(symbol_e token_type, data_type_e expected_type);
    };
}

#endif // __ROW_VALIDATOR_HPP__
