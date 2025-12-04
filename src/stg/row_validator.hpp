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
        RowValidator(const TableSchema &schema) : _schema(schema) {}

        ValidationResult Validate(
            const std::vector<std::string> &columns,
            const std::vector<std::shared_ptr<Token>> &values) const;

    private:
        TableSchema _schema;

        ValidationResult ValidateColumnCount(
            const std::vector<std::string> &columns,
            const std::vector<std::shared_ptr<Token>> &values) const;
        ValidationResult ValidateColumnExists(std::string_view column_name) const;
        ValidationResult ValidateColumnType(
            std::string_view column_name,
            const data_type_t &expected_type,
            const Token &value) const;
        ValidationResult ValidateColumnLength(
            std::string_view column_name,
            const data_type_t &expected_type,
            const Token &value) const;
        ValidationResult ValidateColumn(
            const std::string &column_name,
            const Token &value) const;
        bool ValidTypeForColumn(symbol_e token_type, data_type_e expected_type) const;
    };
}

#endif // __ROW_VALIDATOR_HPP__
