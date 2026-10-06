#ifndef SIMCOM_TEST_MINI_JSON_HPP
#define SIMCOM_TEST_MINI_JSON_HPP

// ---------------------------------------------------------------------------
// mini_json.hpp - Memory ownership contract:
//
//   JsonValue* parse(const char* json, size_t len);
//     - Returns a heap-allocated tree of JsonValue nodes.
//     - The CALLER OWNS the returned pointer.
//     - The CALLER MUST call free_json(root) exactly once.
//     - On parse failure, returns nullptr; no allocation is leaked.
//
//   void free_json(JsonValue* root);
//     - Recursively frees the entire tree.
//     - Passing nullptr is a no-op.
//
// Rationale: this header is test-only and lives under test/support/.
// It is EXEMPT from the no-malloc rule that applies to include/simcom/*.hpp.
// ---------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace mini_json {

enum class Type { Null, Bool, Number, String, Array, Object };

struct JsonValue {
    Type type = Type::Null;
    bool boolean = false;
    double number = 0.0;
    std::string str;
    std::vector<JsonValue*> array;
    std::vector<std::pair<std::string, JsonValue*>> object;
};

// Forward declaration - defined below, used inside detail::Parser.
inline void free_json(JsonValue* root);

namespace detail {

struct Parser {
    const char* p;
    const char* end;

    void skip_ws() {
        while (p < end && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) ++p;
    }

    bool consume(char c) {
        skip_ws();
        if (p < end && *p == c) { ++p; return true; }
        return false;
    }

    JsonValue* parse_value() {
        skip_ws();
        if (p >= end) return nullptr;
        char c = *p;
        if (c == 'n') return parse_literal("null", Type::Null);
        if (c == 't') return parse_literal("true", Type::Bool, true);
        if (c == 'f') return parse_literal("false", Type::Bool, false);
        if (c == '"') return parse_string();
        if (c == '[') return parse_array();
        if (c == '{') return parse_object();
        if (c == '-' || (c >= '0' && c <= '9')) return parse_number();
        return nullptr;
    }

    JsonValue* parse_literal(const char* lit, Type t, bool b=false) {
        std::size_t n = std::strlen(lit);
        if (static_cast<std::size_t>(end - p) < n) return nullptr;
        if (std::strncmp(p, lit, n) != 0) return nullptr;
        p += n;
        auto* v = new JsonValue();
        v->type = t;
        v->boolean = b;
        return v;
    }

    JsonValue* parse_string() {
        if (!consume('"')) return nullptr;
        auto* v = new JsonValue();
        v->type = Type::String;
        while (p < end && *p != '"') {
            if (*p == '\\' && p + 1 < end) {
                ++p;
                switch (*p) {
                    case '"':  v->str.push_back('"');  break;
                    case '\\': v->str.push_back('\\'); break;
                    case '/':  v->str.push_back('/');  break;
                    case 'n':  v->str.push_back('\n'); break;
                    case 't':  v->str.push_back('\t'); break;
                    case 'r':  v->str.push_back('\r'); break;
                    default:   v->str.push_back(*p);   break;
                }
                ++p;
            } else {
                v->str.push_back(*p++);
            }
        }
        if (p >= end) { free_json(v); return nullptr; }
        ++p;
        return v;
    }

    JsonValue* parse_number() {
        const char* start = p;
        if (*p == '-') ++p;
        while (p < end && *p >= '0' && *p <= '9') ++p;
        if (p < end && *p == '.') {
            ++p;
            while (p < end && *p >= '0' && *p <= '9') ++p;
        }
        if (p < end && (*p == 'e' || *p == 'E')) {
            ++p;
            if (p < end && (*p == '+' || *p == '-')) ++p;
            while (p < end && *p >= '0' && *p <= '9') ++p;
        }
        std::string s(start, p - start);
        auto* v = new JsonValue();
        v->type = Type::Number;
        v->number = std::strtod(s.c_str(), nullptr);
        return v;
    }

    JsonValue* parse_array() {
        if (!consume('[')) return nullptr;
        auto* v = new JsonValue();
        v->type = Type::Array;
        skip_ws();
        if (consume(']')) return v;
        while (true) {
            JsonValue* item = parse_value();
            if (!item) { free_json(v); return nullptr; }
            v->array.push_back(item);
            skip_ws();
            if (consume(']')) return v;
            if (!consume(',')) { free_json(v); return nullptr; }
        }
    }

    JsonValue* parse_object() {
        if (!consume('{')) return nullptr;
        auto* v = new JsonValue();
        v->type = Type::Object;
        skip_ws();
        if (consume('}')) return v;
        while (true) {
            JsonValue* key = parse_string();
            if (!key) { free_json(v); return nullptr; }
            if (!consume(':')) { free_json(key); free_json(v); return nullptr; }
            JsonValue* val = parse_value();
            if (!val) { free_json(key); free_json(v); return nullptr; }
            v->object.emplace_back(key->str, val);
            free_json(key);
            skip_ws();
            if (consume('}')) return v;
            if (!consume(',')) { free_json(v); return nullptr; }
        }
    }
};

} // namespace detail

inline void free_json(JsonValue* root) {
    if (!root) return;
    for (auto* c : root->array) free_json(c);
    for (auto& kv : root->object) free_json(kv.second);
    delete root;
}

inline JsonValue* parse(const char* json, std::size_t len) {
    if (!json) return nullptr;
    detail::Parser parser{json, json + len};
    JsonValue* v = parser.parse_value();
    if (!v) return nullptr;
    parser.skip_ws();
    if (parser.p != parser.end) { free_json(v); return nullptr; }
    return v;
}

} // namespace mini_json

#endif // SIMCOM_TEST_MINI_JSON_HPP
// SPDX-License-Identifier: MIT