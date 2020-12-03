#include "Lexer.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include <array>

namespace ModelLoader
{
	class Parser
	{
	public:
		class Result
		{
		public:
			std::vector<glm::dvec3> m_Vertices;
			std::vector<std::array<std::size_t, 3>> m_Surfaces;
			std::vector<sf::Color> m_Colors;
		};

		Lexer m_Lexer;
		Result m_Result;

		Parser(Lexer& lexer);

		void Parse();
		void ConsumeColor();
		void ConsumeSurface();
		void ConsumeVertex();
		Lexer::Token ExpectNext(TokenType type);
	};

}