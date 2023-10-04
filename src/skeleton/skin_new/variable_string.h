//
// Created by cpasjuste on 24/10/22.
//

#ifndef RR_LAUNCHER_VARIABLE_STRING_H
#define RR_LAUNCHER_VARIABLE_STRING_H

#include <string>
#include <vector>

namespace RRLauncher {
    class VariableString {
    public:
        struct VariableWord {
            std::string text;
            std::string value;
            bool isVar = false;
        };

        explicit VariableString() = default;

        explicit VariableString(const std::string &str);

        std::string toString(Skin *skin, const std::string &dataName, int index);

        bool hasVars() { return m_has_var; }

    private:
        bool m_has_var = false;
        std::string m_string;
        std::vector<VariableWord> m_words;
    };
}

#endif //RR_LAUNCHER_VARIABLE_STRING_H
