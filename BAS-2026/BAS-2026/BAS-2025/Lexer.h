#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace Lexer {

struct SourceSpan {
    uint32_t offset = 0;
    uint32_t length = 0;
    int line = 1;
    int col = 1;
};

enum class TokenKind : uint8_t {
    KwUint,
    KwString,
    KwChar,
    KwFunction,
    KwMain,
    KwReturn,
    KwPrint,
    KwSwitch,
    KwCase,
    KwDefault,
    KwBreak,
    Identifier,
    IntLiteral,
    CharLiteral,
    StringLiteral,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Semi,
    Comma,
    Colon,
    Assign,
    Plus,
    Minus,
    Star,
    Slash,
    BitAnd,
    BitOr,
    BitNot,
    End,
    Unknown
};

const char* TokenKindName(TokenKind kind);

struct Token {
    TokenKind kind = TokenKind::Unknown;
    SourceSpan span;
    int symbolId = -1;
    int literalId = -1;
};

enum class DataType {
    Unknown,
    UInt,
    String,
    Char,
    Void
};

const char* DataTypeName(DataType type);

struct Literal {
    int id = -1;
    DataType type = DataType::Unknown;
    uint32_t uintValue = 0;
    unsigned char charValue = 0;
    std::string stringValue;
    SourceSpan span;
};

enum class SymbolKind {
    Unknown,
    Variable,
    Parameter,
    Function
};

const char* SymbolKindName(SymbolKind kind);

struct Symbol {
    int id = -1;
    std::string name;
    SymbolKind kind = SymbolKind::Unknown;
    DataType type = DataType::Unknown;
    int scopeId = 0;
    int paramIndex = -1;
    SourceSpan declSpan;
};

enum class DiagnosticCode : int {
    UnexpectedCharacter = 2000,
    UnterminatedString = 2001,
    UnterminatedComment = 2002,
    InvalidEscape = 2003,
    InvalidNumber = 2004,
    NumberOutOfRange = 2005,
    InvalidNumberSuffix = 2006,
    FileOpenError = 2007
};

struct Diagnostic {
    DiagnosticCode code = DiagnosticCode::UnexpectedCharacter;
    SourceSpan span;
    std::string message;
};

struct LexResult {
    std::string fileName;
    std::string source;
    std::vector<Token> tokens;
    std::vector<Literal> literals;
    std::vector<Symbol> symbols;
    std::vector<Diagnostic> diagnostics;

    bool ok() const { return diagnostics.empty(); }
    const Literal* findLiteral(int id) const;
    const Symbol* findSymbol(int id) const;
};

LexResult Tokenize(std::string source, std::string fileName = "<input>");
LexResult TokenizeFile(const wchar_t* path);

void Dump(const LexResult& result, std::ostream& os);

} // namespace Lexer
