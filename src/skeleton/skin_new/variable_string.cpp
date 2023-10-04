//
// Created by cpasjuste on 24/10/22.
//

#include "skin.h"
#include "variable_string.h"

using namespace RRLauncher;

VariableString::VariableString(const std::string &str) {
    m_string = str;
    m_has_var = c2d::Utility::contains(m_string, "${");
    if (m_has_var) {
        // split string
        std::vector<std::string> split = c2d::Utility::split(m_string, "${");
        for (const auto &s: split) {
            size_t start = s.find('}');
            if (start != std::string::npos) {
                // this is a var...
                m_words.push_back({s.substr(0, start), "", true});
                if (start + 1 != s.length()) {
                    std::string t = s.substr(start + 1, s.length() - 1);
                    m_words.push_back({t, t, false});
                }
            } else {
                m_words.push_back({s, s, false});
            }
        }
    }
}

std::string VariableString::toString(Skin *skin, const std::string &dataName, int index) {
    bool changed = false;

    if (m_has_var) {
        for (auto &word: m_words) {
            if (word.isVar) {
                std::string value = skin->getVar(word.text, dataName, index);
                if (value != word.value) {
                    word.value = value;
                    changed = true;
                }
            }
        }

        if (changed) {
            m_string.clear();
            for (const auto &w: m_words) {
                m_string += w.value;
            }
        }
    }

    return m_string;
}
