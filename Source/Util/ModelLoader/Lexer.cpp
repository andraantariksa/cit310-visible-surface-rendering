#include "Lexer.hpp"

#include <fstream>
#include <stdexcept>
#include <istream>
#include <cctype>

namespace ModelLoader
{
    Lexer::Lexer(std::basic_istream<char>& stream) :
        m_Stream(stream),
        m_CurrentPos(0),
        m_ColumnNumber(1),
        m_LineNumber(1)
    {
        m_CurrentChar = m_Stream.get();
        if (m_Stream.eof())
        {
            m_Stream.clear(std::ios_base::eofbit);
            m_CurrentChar = '\0';
        }
    }

    Lexer::Token Lexer::SingleSymbol(TokenType type)
    {
        Lexer::Token token;
        token.m_Type = type;

        NextChar();

        return token;
    }

    char Lexer::NextChar()
    {
        m_ColumnNumber += 1;
        m_CurrentPos += 1;
        m_CurrentChar = m_Stream.get();

        if (m_Stream.eof())
        {
            m_Stream.clear(std::ios_base::eofbit);
            m_CurrentChar = '\0';
        }

        return CurrentChar();
    }

    inline char Lexer::CurrentChar()
    {
        return m_CurrentChar;
    }

    Lexer::Token Lexer::NextToken()
    {
        auto start = m_CurrentPos;

        switch (CurrentChar())
        {
        case ' ':
        case '\t': {
            ConsumeWhitespace();
            m_Token.m_Type = TokenType::Whitespace;
            break;
        }
        case '\n': {
            m_ColumnNumber = 0;
            m_LineNumber += 1;

            NextChar();
            m_Token.m_Type = TokenType::Newline;
            break;
        }
        case '\r':
            if (NextChar() == '\n')
            {
                m_ColumnNumber = 0;
                m_LineNumber += 1;

                NextChar();
            }
            else {
                throw std::runtime_error("Expected char \\n");
            }
            break;
        case '\0': {
            return SingleSymbol(TokenType::EndOfFile);
        }
        case '-':
            if (IsNumber(NextChar()))
            {
                ConsumeInteger(true);
            }
            else
            {
                throw std::runtime_error("Error, expecting a number");
            }
            break;
        case '#':
            SkipUntilNewline();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            ConsumeInteger();
            break;
        }
        case 'v': {
            return SingleSymbol(TokenType::VertexChar);
            break;
        }
        case 'f': {
            return SingleSymbol(TokenType::SurfaceChar);
            break;
        }
        case 'c': {
            return SingleSymbol(TokenType::ColorChar);
            break;
        }
        default:
            throw std::runtime_error("Unidentified token");
        }

        return m_Token;
    }

    void Lexer::ConsumeWhitespace()
    {
        while (IsWhitespace(NextChar()))
        {
        }
        m_Token.m_Type = TokenType::Whitespace;
    }

    void Lexer::ConsumeInteger(bool negative)
    {
        std::string number;
        number += CurrentChar();

        while (IsNumber(NextChar()))
        {
            number += CurrentChar();
        }

        if (CurrentChar() == '.')
        {
            number += '.';
            ConsumeDoubleRest(number, negative);
            return;
        }

        m_Token.m_Type = TokenType::Integer;
        m_Token.m_Int = std::stoi(number.c_str()) * (negative ? -1 : 1);
    }

    void Lexer::SkipUntilNewline()
    {
        while (true)
        {
            auto c = NextChar();
            if (c == '\n')
            {
                return;
            }
            else if (c == '\r' && PeekChar() == '\n')
            {
                NextChar();
                return;
            }
        }
    }

    char Lexer::PeekChar()
    {
        return m_Stream.peek();
    }

    void Lexer::ConsumeDoubleRest(std::string& number, bool negative)
    {
        while (IsNumber(NextChar()))
        {
            number += CurrentChar();
        }

        m_Token.m_Type = TokenType::Double;
        m_Token.m_Double = std::stod(number.c_str()) * (negative ? -1.0 : 1.0);
    }

    bool Lexer::IsWhitespace(char c)
    {
        switch (c)
        {
        case ' ':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    inline bool Lexer::IsNumber(char c)
    {
        return std::isdigit(c);
    }

}