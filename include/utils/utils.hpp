#include <string>

namespace engine
{
    class Utils
    {
        public:
            Utils() = delete;

            /// string functions modified from:
            /// https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
            static inline void LeftTrim(std::string& s)
            {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch)
                {
                    return !std::isspace(ch);
                }));
            }

            static inline void RightTrim(std::string& s)
            {
                s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch)
                {
                    return !std::isspace(ch);
                }).base(), s.end());
            }

            static inline void Trim(std::string& s)
            {
                RightTrim(s);
                LeftTrim(s);
            }

            static inline bool IsPositiveInt(std::string& s)
            {
                return s.find_first_not_of("0123456789") == std::string::npos;
            }

            static inline bool IsNegativeInt(std::string& s)
            {
                // need at least 2 chars for a negative int (e.g. -1)
                if (s.length() < 2)
                {
                    return false;
                }

                // if negative, must start with '-' char and remaining
                // characters must form a positive integer
                auto int_substr = s.substr(1);
                return s[0] == '-' && IsPositiveInt(int_substr);
            }
    };
};