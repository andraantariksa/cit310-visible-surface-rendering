#include "Parser.hpp"

#include <cassert>

#include "Lexer.hpp"

namespace ModelLoader
{
	Parser::Parser(Lexer& lexer):
		m_Lexer(lexer)
	{
	}

	void Parser::Parse()
	{
		while (true)
		{
			auto token = m_Lexer.NextToken();
			if (token.m_Type == TokenType::EndOfFile)
			{
				break;
			}

			switch (token.m_Type)
			{
			case TokenType::ColorChar:
				ExpectNext(TokenType::Whitespace);
				ConsumeColor();
				break;
			case TokenType::SurfaceChar:
				ExpectNext(TokenType::Whitespace);
				ConsumeSurface();
				break;
			case TokenType::VertexChar:
				ExpectNext(TokenType::Whitespace);
				ConsumeVertex();
			default:
				break;
			}
		}

		assert(m_Result.m_Colors.size() == m_Result.m_Surfaces.size() && "Total color and surface does not match!");
	}

	void Parser::ConsumeColor()
	{
		auto firstInteger = ExpectNext(TokenType::Integer);
		ExpectNext(TokenType::Whitespace);
		auto secondInteger = ExpectNext(TokenType::Integer);
		ExpectNext(TokenType::Whitespace);
		auto thirdInteger = ExpectNext(TokenType::Integer);
		m_Result.m_Colors.push_back({ (sf::Uint8)firstInteger.m_Int, (sf::Uint8)secondInteger.m_Int, (sf::Uint8)thirdInteger.m_Int });
	}

	void Parser::ConsumeSurface()
	{
		auto firstInteger = ExpectNext(TokenType::Integer);
		ExpectNext(TokenType::Whitespace);
		auto secondInteger = ExpectNext(TokenType::Integer);
		ExpectNext(TokenType::Whitespace);
		auto thirdInteger = ExpectNext(TokenType::Integer);
		m_Result.m_Surfaces.push_back({ (std::size_t)firstInteger.m_Int, (std::size_t)secondInteger.m_Int, (std::size_t)thirdInteger.m_Int });
	}

	void Parser::ConsumeVertex()
	{
		auto firstDouble = ExpectNext(TokenType::Double);
		ExpectNext(TokenType::Whitespace);
		auto secondDouble = ExpectNext(TokenType::Double);
		ExpectNext(TokenType::Whitespace);
		auto thirdDouble = ExpectNext(TokenType::Double);
		m_Result.m_Vertices.push_back({ firstDouble.m_Double, secondDouble.m_Double, thirdDouble.m_Double });
	}

	Lexer::Token Parser::ExpectNext(TokenType type)
	{
		Lexer::Token token = m_Lexer.NextToken();
		assert(token.m_Type == type && "Token is not expected");
		return std::move(token);
	}
}
