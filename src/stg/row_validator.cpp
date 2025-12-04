#include "stg/row_validator.hpp"

namespace litedb
{
    ValidationResult ValidationResult::Ok()
    {
        return {true, ""};
    }

    ValidationResult ValidationResult::Error(std::string_view message)
    {
        return {false, std::string(message)};
    }

    ValidationResult RowValidator::Validate(
        const std::vector<std::string> &columns,
        const std::vector<std::shared_ptr<Token>> &values) const
    {
        // if no columns specified, validate against schema order
        if (columns.empty())
        {
            if (values.size() != _schema.columns.size())
            {
                return ValidationResult::Error(
                    "Column count mismatch: expected " +
                    std::to_string(_schema.columns.size()) + ", got " +
                    std::to_string(values.size()));
            }

            // validate each value against schema column in order
            for (size_t i = 0; i < values.size(); i++)
            {
                const auto &schema_col = _schema.columns[i];
                auto valid_type = ValidateColumnType(schema_col.name, schema_col.type, *values[i]);
                if (!valid_type.success)
                {
                    return valid_type;
                }

                auto valid_length = ValidateColumnLength(schema_col.name, schema_col.type, *values[i]);
                if (!valid_length.success)
                {
                    return valid_length;
                }
            }

            return ValidationResult::Ok();
        }

        auto valid_count = ValidateColumnCount(columns, values);
        if (!valid_count.success)
        {
            return valid_count;
        }

        for (size_t i = 0; i < columns.size(); i++)
        {
            auto valid_col = ValidateColumn(columns[i], *values[i]);
            if (!valid_col.success)
            {
                return valid_col;
            }
        }

        return ValidationResult::Ok();
    }

    ValidationResult RowValidator::ValidateColumnCount(
        const std::vector<std::string> &columns,
        const std::vector<std::shared_ptr<Token>> &values) const
    {
        if (columns.size() != values.size())
        {
            return ValidationResult::Error(
                "Column count (" + std::to_string(columns.size()) +
                ") does not match value count (" + std::to_string(values.size()) + ")");
        }
        return ValidationResult::Ok();
    }

    ValidationResult RowValidator::ValidateColumnExists(std::string_view column_name) const
    {
        if (!_schema.HasColumn(std::string(column_name)))
        {
            return ValidationResult::Error("Unknown column: '" + std::string(column_name) + "'");
        }
        return ValidationResult::Ok();
    }

    ValidationResult RowValidator::ValidateColumnType(
        std::string_view column_name,
        const data_type_t &expected_type,
        const Token &value) const
    {
        if (!ValidTypeForColumn(value.GetType(), expected_type.name))
        {
            return ValidationResult::Error(
                "Type mismatch for column '" + std::string(column_name) + "': expected " +
                std::string(DataTypeToString(expected_type.name)) + ", got " +
                symbol_e_map[value.GetType()]);
        }

        return ValidationResult::Ok();
    }

    ValidationResult RowValidator::ValidateColumnLength(
        std::string_view column_name,
        const data_type_t &expected_type,
        const Token &value) const
    {
        if (expected_type.name != data_type_e::CHAR)
        {
            return ValidationResult::Ok();
        }

        if (value.GetValue().length() > expected_type.length)
        {
            return ValidationResult::Error(
                "Value too long for column '" + std::string(column_name) +
                "': max length is " + std::to_string(expected_type.length) +
                ", got " + std::to_string(value.GetValue().length()));
        }

        return ValidationResult::Ok();
    }

    ValidationResult RowValidator::ValidateColumn(
        const std::string &column_name,
        const Token &value) const
    {
        auto col_exists = ValidateColumnExists(column_name);
        if (!col_exists.success)
        {
            return col_exists;
        }

        const auto *data_type = _schema.GetColumnType(column_name);
        if (data_type == nullptr)
        {
            return ValidationResult::Error("Unknown column: '" + column_name + "'");
        }

        auto valid_type = ValidateColumnType(column_name, *data_type, value);
        if (!valid_type.success)
        {
            return valid_type;
        }

        // check length for CHAR
        auto valid_length = ValidateColumnLength(column_name, *data_type, value);
        if (!valid_length.success)
        {
            return valid_length;
        }

        return ValidationResult::Ok();
    }

    bool RowValidator::ValidTypeForColumn(symbol_e token_type, data_type_e expected_type) const
    {
        switch (expected_type)
        {
        case data_type_e::INT:
            return token_type == symbol_e::LITERAL_INT;

        case data_type_e::FLOAT:
            // For now, only INT literals supported (FLOAT literal not yet in lexer)
            // TODO: Add LITERAL_FLOAT to symbol_e when float parsing is implemented
            return token_type == symbol_e::LITERAL_INT;

        case data_type_e::CHAR:
            return token_type == symbol_e::LITERAL_STRING;

        default:
            return false;
        }
    }
}
