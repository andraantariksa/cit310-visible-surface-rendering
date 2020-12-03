#ifndef __UTIL_MODELLEXER_HPP
#define __UTIL_MODELLEXER_HPP

#include <vector>
#include <istream>

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

namespace ModelLoader
{
	class Lexer
	{
	public:
		// Member data types
		class Token
		{
		public:
			enum class Type
			{
				Whitespace,
				Double,
				Integer,
				VertexChar,
				SurfaceChar,
				ColorChar,
				Newline,
				EndOfFile
			};

			Type m_Type;
			double m_Double;
			long m_Int;
		};

		// Member variables
		Token m_Token;
		char m_CurrentChar;
		std::size_t m_LineNumber;
		std::size_t m_ColumnNumber;
		std::iostream::pos_type m_CurrentPos;
		std::basic_istream<char>& m_Stream;

		// Member functions
		Lexer(std::basic_istream<char>& stream);

		Token NextToken();

		void ConsumeDoubleRest(std::string& frontNumber, bool negative = false);
		void ConsumeInteger(bool negative = false);
		void ConsumeWhitespace();
		Token SingleSymbol(Token::Type type);

		char NextChar();
		char PeekChar();
		char CurrentChar();
		void SkipUntilNewline();

		bool IsWhitespace(char c);
		bool IsNumber(char c);
	};

	using TokenType = Lexer::Token::Type;
}

#endif
