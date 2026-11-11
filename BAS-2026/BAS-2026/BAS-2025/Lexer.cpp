#include "Lexer.h"

#include <cctype>
#include <cstdint>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Lexer {

namespace {

bool IsDigit(int c) {
    return c >= '0' && c <= '9';
}

bool IsHexDigit(int c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

int HexValue(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return c - 'A' + 10;
}

bool IsIdentStart(int c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool IsIdentContinue(int c) {
    return IsIdentStart(c) || IsDigit(c);
}

bool IsSpace(int c) {
    return c != -1 && std::isspace(static_cast<unsigned char>(c)) != 0;
}

TokenKind KeywordKind(const std::string& text) {
    static const std::unordered_map<std::string, TokenKind> keywords = {
        {"uint", TokenKind::KwUint},
        {"string", TokenKind::KwString},
        {"char", TokenKind::KwChar},
        {"function", TokenKind::KwFunction},
        {"main", TokenKind::KwMain},
        {"return", TokenKind::KwReturn},
        {"print", TokenKind::KwPrint},
        {"switch", TokenKind::KwSwitch},
        {"case", TokenKind::KwCase},
        {"default", TokenKind::KwDefault},
        {"break", TokenKind::KwBreak}
    };

    auto it = keywords.find(text);
    return it == keywords.end() ? TokenKind::Unknown : it->second;
}

std::string EscapeForDump(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    for (unsigned char c : text) {
        switch (c) {
        case '\\': result += "\\\\"; break;
        case '\n': result += "\\n"; break;
        case '\r': result += "\\r"; break;
        case '\t': result += "\\t"; break;
        case '\0': result += "\\0"; break;
        case '\'': result += "\\'"; break;
        case '"': result += "\\\""; break;
        default:
            if (c >= 32 && c < 127) {
                result.push_back(static_cast<char>(c));
            }
            else {
                std::ostringstream ss;
                ss << "\\x" << std::hex << std::uppercase;
                if (c < 16) ss << '0';
                ss << static_cast<int>(c);
                result += ss.str();
            }
            break;
        }
    }

    return result;
}

std::string TokenText(const LexResult& result, const Token& token) {
    if (token.kind == TokenKind::End) return {};
    if (token.span.offset > result.source.size()) return {};
    if (token.span.length > result.source.size() - token.span.offset) return {};
    return result.source.substr(token.span.offset, token.span.length);
}

std::string NarrowPath(const wchar_t* path) {
    if (path == nullptr) return "<input>";

    std::string result;
    for (const wchar_t* p = path; *p != L'\0'; ++p) {
        if (*p >= 0x20 && *p < 0x7F) {
            result.push_back(static_cast<char>(*p));
        }
        else {
            result.push_back('?');
        }
    }
    return result;
}

class LexerEngine {
public:
    explicit LexerEngine(LexResult& result)
        : result_(result), source_(result.source) {
        if (source_.size() >= 3 &&
            static_cast<unsigned char>(source_[0]) == 0xEF &&
            static_cast<unsigned char>(source_[1]) == 0xBB &&
            static_cast<unsigned char>(source_[2]) == 0xBF) {
            pos_ = 3;
        }
    }

    void Run() {
        while (true) {
            SkipWhitespace();
            if (AtEnd()) {
                AddToken(TokenKind::End, StartSpan());
                return;
            }

            const int c = Peek();
            const SourceSpan span = StartSpan();

            if (IsIdentStart(c)) {
                LexIdentifier();
            }
            else if (IsDigit(c)) {
                LexNumber();
            }
            else if (c == '\'') {
                LexQuotedLiteral();
            }
            else if (c == '/' && Peek(1) == '/') {
                SkipLineComment();
            }
            else if (c == '/' && Peek(1) == '*') {
                SkipBlockComment(span);
            }
            else {
                LexPunctuation(span, c);
            }
        }
    }

private:
    bool AtEnd() const {
        return pos_ >= source_.size();
    }

    int Peek(std::size_t ahead = 0) const {
        const std::size_t index = pos_ + ahead;
        return index < source_.size()
            ? static_cast<unsigned char>(source_[index])
            : -1;
    }

    void Advance() {
        if (AtEnd()) return;

        const unsigned char c = static_cast<unsigned char>(source_[pos_++]);
        if (c == '\r') {
            if (!AtEnd() && source_[pos_] == '\n') {
                ++pos_;
            }
            ++line_;
            col_ = 1;
        }
        else if (c == '\n') {
            ++line_;
            col_ = 1;
        }
        else {
            ++col_;
        }
    }

    SourceSpan StartSpan() const {
        SourceSpan span;
        span.offset = static_cast<uint32_t>(pos_);
        span.length = 0;
        span.line = line_;
        span.col = col_;
        return span;
    }

    SourceSpan FinishSpan(SourceSpan span) const {
        span.length = static_cast<uint32_t>(pos_ - span.offset);
        return span;
    }

    void SkipWhitespace() {
        while (!AtEnd() && IsSpace(Peek())) {
            Advance();
        }
    }

    void SkipLineComment() {
        Advance();
        Advance();
        while (!AtEnd() && Peek() != '\n' && Peek() != '\r') {
            Advance();
        }
    }

    void SkipBlockComment(SourceSpan start) {
        Advance();
        Advance();
        while (!AtEnd()) {
            if (Peek() == '*' && Peek(1) == '/') {
                Advance();
                Advance();
                return;
            }
            Advance();
        }
        AddDiagnostic(DiagnosticCode::UnterminatedComment, start, "unterminated block comment");
    }

    void LexIdentifier() {
        const SourceSpan start = StartSpan();
        const std::size_t begin = pos_;

        while (!AtEnd() && IsIdentContinue(Peek())) {
            Advance();
        }

        const std::string text = source_.substr(begin, pos_ - begin);
        const SourceSpan span = FinishSpan(start);
        const TokenKind keyword = KeywordKind(text);

        if (keyword != TokenKind::Unknown) {
            AddToken(keyword, span);
            return;
        }

        const int symbolId = InternSymbol(text, span);
        AddToken(TokenKind::Identifier, span, symbolId, -1);
    }

    void LexNumber() {
        SourceSpan span = StartSpan();
        const bool isHex = Peek() == '0' && (Peek(1) == 'x' || Peek(1) == 'X');

        if (isHex) {
            Advance();
            Advance();

            if (!IsHexDigit(Peek())) {
                AddDiagnostic(DiagnosticCode::InvalidNumber, FinishSpan(span),
                    "hexadecimal literal must contain at least one digit");
                return;
            }
        }

        uint64_t value = 0;
        bool overflow = false;

        if (isHex) {
            while (!AtEnd() && IsHexDigit(Peek())) {
                value = value * 16 + static_cast<uint32_t>(HexValue(Peek()));
                if (value > std::numeric_limits<uint32_t>::max()) {
                    overflow = true;
                }
                Advance();
            }
        }
        else {
            while (!AtEnd() && IsDigit(Peek())) {
                value = value * 10 + static_cast<uint32_t>(Peek() - '0');
                if (value > std::numeric_limits<uint32_t>::max()) {
                    overflow = true;
                }
                Advance();
            }
        }

        if (!AtEnd() && IsIdentStart(Peek())) {
            AddDiagnostic(DiagnosticCode::InvalidNumberSuffix, FinishSpan(span),
                "invalid suffix after numeric literal");
            while (!AtEnd() && IsIdentContinue(Peek())) {
                Advance();
            }
        }

        span = FinishSpan(span);

        if (overflow) {
            AddDiagnostic(DiagnosticCode::NumberOutOfRange, span,
                "integer literal is out of uint range");
            value = std::numeric_limits<uint32_t>::max();
        }

        const int literalId = InternUIntLiteral(static_cast<uint32_t>(value), span);
        AddToken(TokenKind::IntLiteral, span, -1, literalId);
    }

    void LexQuotedLiteral() {
        SourceSpan span = StartSpan();
        Advance();

        std::string value;
        bool closed = false;
        bool escapedLineBreak = false;

        while (!AtEnd()) {
            const int c = Peek();

            if (c == '\'') {
                Advance();
                closed = true;
                break;
            }

            if (c == '\n' || c == '\r') {
                break;
            }

            if (c == '\\') {
                const SourceSpan escapeSpan = StartSpan();
                Advance();

                if (AtEnd()) {
                    break;
                }

                const int escaped = Peek();
                Advance();

                switch (escaped) {
                case 'n': value.push_back('\n'); break;
                case 't': value.push_back('\t'); break;
                case 'r': value.push_back('\r'); break;
                case '0': value.push_back('\0'); break;
                case '\\': value.push_back('\\'); break;
                case '\'': value.push_back('\''); break;
                case '"': value.push_back('"'); break;
                case '\n':
                case '\r':
                    AddDiagnostic(DiagnosticCode::InvalidEscape, escapeSpan,
                        "line break is not allowed in an escape sequence");
                    escapedLineBreak = true;
                    break;
                default:
                    AddDiagnostic(DiagnosticCode::InvalidEscape, escapeSpan,
                        "unknown escape sequence");
                    value.push_back(static_cast<char>(escaped));
                    break;
                }

                if (escapedLineBreak) {
                    break;
                }
            }
            else {
                value.push_back(static_cast<char>(c));
                Advance();
            }
        }

        if (!closed) {
            AddDiagnostic(DiagnosticCode::UnterminatedString, span,
                "unterminated character or string literal");
            return;
        }

        span = FinishSpan(span);

        if (value.size() == 1) {
            const int literalId = InternCharLiteral(static_cast<unsigned char>(value[0]), span);
            AddToken(TokenKind::CharLiteral, span, -1, literalId);
        }
        else {
            const int literalId = InternStringLiteral(value, span);
            AddToken(TokenKind::StringLiteral, span, -1, literalId);
        }
    }

    void LexPunctuation(SourceSpan span, int c) {
        TokenKind kind = TokenKind::Unknown;

        switch (c) {
        case '(': kind = TokenKind::LParen; break;
        case ')': kind = TokenKind::RParen; break;
        case '{': kind = TokenKind::LBrace; break;
        case '}': kind = TokenKind::RBrace; break;
        case ';': kind = TokenKind::Semi; break;
        case ',': kind = TokenKind::Comma; break;
        case ':': kind = TokenKind::Colon; break;
        case '=': kind = TokenKind::Assign; break;
        case '+': kind = TokenKind::Plus; break;
        case '-': kind = TokenKind::Minus; break;
        case '*': kind = TokenKind::Star; break;
        case '/': kind = TokenKind::Slash; break;
        case '&': kind = TokenKind::BitAnd; break;
        case '|': kind = TokenKind::BitOr; break;
        case '~': kind = TokenKind::BitNot; break;
        default: break;
        }

        Advance();
        span = FinishSpan(span);

        if (kind == TokenKind::Unknown) {
            std::ostringstream message;
            message << "unexpected character 0x" << std::hex << std::uppercase
                    << (c & 0xFF);
            AddDiagnostic(DiagnosticCode::UnexpectedCharacter, span, message.str());
            return;
        }

        AddToken(kind, span);
    }

    void AddToken(TokenKind kind, SourceSpan span, int symbolId = -1, int literalId = -1) {
        Token token;
        token.kind = kind;
        token.span = span;
        token.symbolId = symbolId;
        token.literalId = literalId;
        result_.tokens.push_back(token);
    }

    void AddDiagnostic(DiagnosticCode code, SourceSpan span, std::string message) {
        Diagnostic diagnostic;
        diagnostic.code = code;
        diagnostic.span = span;
        diagnostic.message = std::move(message);
        result_.diagnostics.push_back(std::move(diagnostic));
    }

    int InternSymbol(const std::string& name, SourceSpan span) {
        auto it = symbolByName_.find(name);
        if (it != symbolByName_.end()) {
            return it->second;
        }

        Symbol symbol;
        symbol.id = static_cast<int>(result_.symbols.size());
        symbol.name = name;
        symbol.kind = SymbolKind::Unknown;
        symbol.type = DataType::Unknown;
        symbol.declSpan = span;

        const int id = symbol.id;
        result_.symbols.push_back(std::move(symbol));
        symbolByName_.emplace(name, id);
        return id;
    }

    int InternUIntLiteral(uint32_t value, SourceSpan span) {
        auto it = uintLiterals_.find(value);
        if (it != uintLiterals_.end()) {
            return it->second;
        }

        Literal literal;
        literal.id = static_cast<int>(result_.literals.size());
        literal.type = DataType::UInt;
        literal.uintValue = value;
        literal.span = span;

        const int id = literal.id;
        result_.literals.push_back(std::move(literal));
        uintLiterals_.emplace(value, id);
        return id;
    }

    int InternCharLiteral(unsigned char value, SourceSpan span) {
        auto it = charLiterals_.find(value);
        if (it != charLiterals_.end()) {
            return it->second;
        }

        Literal literal;
        literal.id = static_cast<int>(result_.literals.size());
        literal.type = DataType::Char;
        literal.charValue = value;
        literal.span = span;

        const int id = literal.id;
        result_.literals.push_back(std::move(literal));
        charLiterals_.emplace(value, id);
        return id;
    }

    int InternStringLiteral(const std::string& value, SourceSpan span) {
        auto it = stringLiterals_.find(value);
        if (it != stringLiterals_.end()) {
            return it->second;
        }

        Literal literal;
        literal.id = static_cast<int>(result_.literals.size());
        literal.type = DataType::String;
        literal.stringValue = value;
        literal.span = span;

        const int id = literal.id;
        result_.literals.push_back(std::move(literal));
        stringLiterals_.emplace(value, id);
        return id;
    }

    LexResult& result_;
    const std::string& source_;
    std::size_t pos_ = 0;
    int line_ = 1;
    int col_ = 1;

    std::unordered_map<std::string, int> symbolByName_;
    std::unordered_map<uint32_t, int> uintLiterals_;
    std::unordered_map<unsigned char, int> charLiterals_;
    std::unordered_map<std::string, int> stringLiterals_;
};

} // namespace

const char* TokenKindName(TokenKind kind) {
    switch (kind) {
    case TokenKind::KwUint: return "KwUint";
    case TokenKind::KwString: return "KwString";
    case TokenKind::KwChar: return "KwChar";
    case TokenKind::KwFunction: return "KwFunction";
    case TokenKind::KwMain: return "KwMain";
    case TokenKind::KwReturn: return "KwReturn";
    case TokenKind::KwPrint: return "KwPrint";
    case TokenKind::KwSwitch: return "KwSwitch";
    case TokenKind::KwCase: return "KwCase";
    case TokenKind::KwDefault: return "KwDefault";
    case TokenKind::KwBreak: return "KwBreak";
    case TokenKind::Identifier: return "Identifier";
    case TokenKind::IntLiteral: return "IntLiteral";
    case TokenKind::CharLiteral: return "CharLiteral";
    case TokenKind::StringLiteral: return "StringLiteral";
    case TokenKind::LParen: return "LParen";
    case TokenKind::RParen: return "RParen";
    case TokenKind::LBrace: return "LBrace";
    case TokenKind::RBrace: return "RBrace";
    case TokenKind::Semi: return "Semi";
    case TokenKind::Comma: return "Comma";
    case TokenKind::Colon: return "Colon";
    case TokenKind::Assign: return "Assign";
    case TokenKind::Plus: return "Plus";
    case TokenKind::Minus: return "Minus";
    case TokenKind::Star: return "Star";
    case TokenKind::Slash: return "Slash";
    case TokenKind::BitAnd: return "BitAnd";
    case TokenKind::BitOr: return "BitOr";
    case TokenKind::BitNot: return "BitNot";
    case TokenKind::End: return "End";
    case TokenKind::Unknown: return "Unknown";
    }

    return "Unknown";
}

const char* DataTypeName(DataType type) {
    switch (type) {
    case DataType::Unknown: return "Unknown";
    case DataType::UInt: return "UInt";
    case DataType::String: return "String";
    case DataType::Char: return "Char";
    case DataType::Void: return "Void";
    }

    return "Unknown";
}

const char* SymbolKindName(SymbolKind kind) {
    switch (kind) {
    case SymbolKind::Unknown: return "Unknown";
    case SymbolKind::Variable: return "Variable";
    case SymbolKind::Parameter: return "Parameter";
    case SymbolKind::Function: return "Function";
    }

    return "Unknown";
}

const Literal* LexResult::findLiteral(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= literals.size()) {
        return nullptr;
    }
    return &literals[static_cast<std::size_t>(id)];
}

const Symbol* LexResult::findSymbol(int id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= symbols.size()) {
        return nullptr;
    }
    return &symbols[static_cast<std::size_t>(id)];
}

LexResult Tokenize(std::string source, std::string fileName) {
    LexResult result;
    result.fileName = std::move(fileName);
    result.source = std::move(source);

    LexerEngine engine(result);
    engine.Run();
    return result;
}

LexResult TokenizeFile(const wchar_t* path) {
    const std::string fileName = NarrowPath(path);
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        LexResult result;
        result.fileName = fileName;

        Diagnostic diagnostic;
        diagnostic.code = DiagnosticCode::FileOpenError;
        diagnostic.message = "cannot open input file";
        result.diagnostics.push_back(std::move(diagnostic));
        return result;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return Tokenize(buffer.str(), fileName);
}

void Dump(const LexResult& result, std::ostream& os) {
    os << "=== LEXER RESULT ===\n";
    os << "file: " << (result.fileName.empty() ? "<input>" : result.fileName) << '\n';
    os << "tokens: " << result.tokens.size()
       << ", symbols: " << result.symbols.size()
       << ", literals: " << result.literals.size()
       << ", diagnostics: " << result.diagnostics.size() << "\n";

    os << "\n--- TOKENS ---\n";
    os << "#\tline:col\tkind\t\tsymbol\tliteral\ttext\n";
    for (std::size_t i = 0; i < result.tokens.size(); ++i) {
        const Token& token = result.tokens[i];
        os << i << '\t'
           << token.span.line << ':' << token.span.col << '\t'
           << TokenKindName(token.kind) << '\t';

        if (token.symbolId >= 0) os << token.symbolId;
        else os << '-';
        os << '\t';

        if (token.literalId >= 0) os << token.literalId;
        else os << '-';
        os << '\t';

        os << EscapeForDump(TokenText(result, token)) << '\n';
    }

    os << "\n--- SYMBOLS ---\n";
    os << "#\tname\tkind\ttype\tdecl\n";
    for (const Symbol& symbol : result.symbols) {
        os << symbol.id << '\t'
           << EscapeForDump(symbol.name) << '\t'
           << SymbolKindName(symbol.kind) << '\t'
           << DataTypeName(symbol.type) << '\t'
           << symbol.declSpan.line << ':' << symbol.declSpan.col << '\n';
    }

    os << "\n--- LITERALS ---\n";
    os << "#\ttype\tvalue\tposition\n";
    for (const Literal& literal : result.literals) {
        os << literal.id << '\t' << DataTypeName(literal.type) << '\t';

        switch (literal.type) {
        case DataType::UInt:
            os << literal.uintValue;
            break;
        case DataType::Char:
            os << '\'' << EscapeForDump(std::string(1, static_cast<char>(literal.charValue)))
               << "' (" << static_cast<unsigned int>(literal.charValue) << ')';
            break;
        case DataType::String:
            os << '"' << EscapeForDump(literal.stringValue) << '"';
            break;
        default:
            os << '?';
            break;
        }

        os << '\t' << literal.span.line << ':' << literal.span.col << '\n';
    }

    os << "\n--- DIAGNOSTICS ---\n";
    if (result.diagnostics.empty()) {
        os << "none\n";
    }
    else {
        for (const Diagnostic& diagnostic : result.diagnostics) {
            os << '[' << static_cast<int>(diagnostic.code) << "] "
               << diagnostic.span.line << ':' << diagnostic.span.col
               << ' ' << diagnostic.message << '\n';
        }
    }
}

} // namespace Lexer
