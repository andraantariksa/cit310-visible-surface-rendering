#include "App.hpp"

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>
#include <glm/glm.hpp>
#include <portable-file-dialogs.h>
#include <fstream>

#include "../Macro.hpp"
#include "../Util/ModelLoader/Lexer.hpp"
#include "../Util/ModelLoader/Parser.hpp"
#include "Camera.hpp"
#include "../ExampleShape.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../System/BaseRenderSystem.hpp"
#include "../Util/Logger.hpp"

// TODO
// Implement file upload
// Idk which format we should use, or we could create a new format that based on CSV?
App::App() :
	m_Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close),
	m_ClearColor(CLEAR_COLOR),
	m_GUIWindowTips(true),
	// -1 means not selecting anything
	m_SelectedEntityIdx(-1),
	m_GUIRenderMethod((int)BaseRenderSystem::RenderMethod::ZBuffer),
	m_GUITransformationMethod((int)TransformationMethod::Translation),
	m_GUITransformStep(10.0f),
	m_SystemRender((BaseRenderSystem::RenderMethod)m_GUIRenderMethod, 500.0),
	m_GUIShowMessageBox(false)
{
	m_Window.setFramerateLimit(20);
	ImGui::SFML::Init(m_Window);
	ImGui::GetIO().IniFilename = nullptr;

	Camera::Direction = { 0.0, 0.0, -1.0 };

	// Initialize default 2 pyramid
	m_Entities[0] = std::make_unique<entt::entity>(m_Registry.create());
	m_Registry.emplace<Shape3DComponent>(*m_Entities[0], Shape3DExample());
	m_Registry.emplace<TransformComponent>(*m_Entities[0]);

	m_SystemTransform.Translate(m_Registry, *m_Entities[0], glm::dvec3(-50.0, 0.0, 0.0));

	m_Entities[1] = std::make_unique<entt::entity>(m_Registry.create());
	m_Registry.emplace<Shape3DComponent>(*m_Entities[1], Shape3DExample());
	m_Registry.emplace<TransformComponent>(*m_Entities[1]);

	m_SystemTransform.Rotate(m_Registry, *m_Entities[1], glm::dvec3(1.0, 0.0, 0.0), 30.0);
	m_SystemTransform.Rotate(m_Registry, *m_Entities[1], glm::dvec3(0.0, 1.0, 0.0), 30.0);
	m_SystemTransform.Translate(m_Registry, *m_Entities[1], glm::dvec3(50.0, 0.0, 0.0));
}

App::~App()
{
	ImGui::SFML::Shutdown();
}

void App::Run()
{
	Update();

	while (m_Window.isOpen())
	{
		sf::Event event;
		bool rightClick = false;
		while (m_Window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				m_Window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
				{
					int nextSelectedEntityIdx = event.key.code - (int)sf::Keyboard::Num1;
					if (m_Entities[nextSelectedEntityIdx])
					{
						m_SelectedEntityIdx = nextSelectedEntityIdx;
					}
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					m_SelectedEntityIdx = -1;
				}
				else if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::P:
					{
						m_SystemRender.Print();
						break;
					}
					default:
						if (m_SelectedEntityIdx != -1 && m_Entities[m_SelectedEntityIdx])
						{
							bool transformation = false;
							switch ((TransformationMethod)m_GUITransformationMethod)
							{
							case TransformationMethod::Translation:
							{
								glm::dvec3 vec(0.0);
								switch (event.key.code)
								{
								case sf::Keyboard::D:
									transformation = true;
									vec.x += m_GUITransformStep;
									break;
								case sf::Keyboard::A:
									transformation = true;
									vec.x -= m_GUITransformStep;
									break;
								case sf::Keyboard::W:
									transformation = true;
									vec.y += m_GUITransformStep;
									break;
								case sf::Keyboard::S:
									transformation = true;
									vec.y -= m_GUITransformStep;
									break;
								case sf::Keyboard::Q:
									transformation = true;
									vec.z += m_GUITransformStep;
									break;
								case sf::Keyboard::E:
									transformation = true;
									vec.z -= m_GUITransformStep;
									break;
								}
								if (transformation)
								{
									m_SystemTransform.Translate(
										m_Registry,
										*m_Entities[m_SelectedEntityIdx],
										vec
									);
									m_SystemRender.Update(m_Registry);
								}
								break;
							}
							case TransformationMethod::Rotation:
							{
								glm::dvec3 vec(0.0);
								double angleSign = 1.0;
								switch (event.key.code)
								{
								case sf::Keyboard::A:
									transformation = true;
									angleSign *= -1.0;
								case sf::Keyboard::D:
									transformation = true;
									vec.y += 1.0;
									break;
								case sf::Keyboard::S:
									transformation = true;
									angleSign *= -1.0;
								case sf::Keyboard::W:
									transformation = true;
									vec.x += 1.0;
									break;
								case sf::Keyboard::Q:
									transformation = true;
									angleSign *= -1.0;
								case sf::Keyboard::E:
									transformation = true;
									vec.z += 1.0;
									break;
								}
								if (transformation)
								{
									m_SystemTransform.Rotate(
										m_Registry,
										*m_Entities[m_SelectedEntityIdx],
										vec,
										m_GUITransformStep * angleSign
									);
									m_SystemRender.Update(m_Registry);
								}
								break;
							}
							};
						}
					};
				}
				else
				{
					switch (event.key.code)
					{
					case sf::Keyboard::F1:
						if (m_GUITransformationMethod != (int)TransformationMethod::Translation)
						{
							m_GUITransformationMethod = (int)TransformationMethod::Translation;
						}
						break;
					case sf::Keyboard::F2:
						if (m_GUITransformationMethod != (int)TransformationMethod::Rotation)
						{
							m_GUITransformationMethod = (int)TransformationMethod::Rotation;
						}
						break;
					case sf::Keyboard::F3:
						if (m_GUIRenderMethod != (int)BaseRenderSystem::RenderMethod::Painter)
						{
							m_GUIRenderMethod = (int)BaseRenderSystem::RenderMethod::Painter;

							m_SystemRender.ChangeRenderMethod(m_Registry, (BaseRenderSystem::RenderMethod)m_GUIRenderMethod);
						}
						break;
					case sf::Keyboard::F4:
						if (m_GUIRenderMethod != (int)BaseRenderSystem::RenderMethod::ZBuffer)
						{
							m_GUIRenderMethod = (int)BaseRenderSystem::RenderMethod::ZBuffer;

							m_SystemRender.ChangeRenderMethod(m_Registry, (BaseRenderSystem::RenderMethod)m_GUIRenderMethod);
						}
						break;
					}
				}
			}
		}

		UpdateInterface();

		m_Window.clear(m_ClearColor);
		Render();

		m_DeltaClock.restart();
	}
}

void App::UpdateInterface()
{
	ImGui::SFML::Update(m_Window, m_DeltaClock.getElapsedTime());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
				int availableSlot = 0;
				for (int i = 0; i < 9; ++i)
				{
					if (!m_Entities[i])
					{
						availableSlot = i;
						break;
					}
				}

				if (availableSlot)
				{
					auto openedFile = pfd::open_file("Choose file to read", "", { "Simple 3D Object", "*.sobj" });
					auto openedFileResult = openedFile.result();
					if (!openedFileResult.empty())
					{
						try {
							std::ifstream file(openedFileResult[0]);
							ModelLoader::Lexer modelLexer(file);
							ModelLoader::Parser modelParser(modelLexer);
							modelParser.Parse();
							std::vector<SurfaceComponent> surfaces;
							for (int i = 0; i < modelParser.m_Result.m_Surfaces.size(); ++i)
							{
								surfaces.push_back(SurfaceComponent(
									modelParser.m_Result.m_Vertices[modelParser.m_Result.m_Surfaces[i][0]],
									modelParser.m_Result.m_Vertices[modelParser.m_Result.m_Surfaces[i][1]],
									modelParser.m_Result.m_Vertices[modelParser.m_Result.m_Surfaces[i][2]],
									modelParser.m_Result.m_Colors[i]));
							}

							m_Entities[availableSlot] = std::make_unique<entt::entity>(m_Registry.create());
							m_Registry.emplace<Shape3DComponent>(*m_Entities[availableSlot], surfaces);
							m_Registry.emplace<TransformComponent>(*m_Entities[availableSlot]);

							m_SystemRender.Update(m_Registry);
						}
						catch (std::exception& ex)
						{
							ShowMessageBox(std::string("Can not load the file.\n") + ex.what());
						}
					}
				}
				else
				{
					ShowMessageBox(std::string("No slot available for the object."));
				}
			}

			if (ImGui::MenuItem("Close"))
			{
				m_Window.close();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Tips"))
			{
				m_GUIWindowTips = true;
			}

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	ImGui::SetNextWindowPos(ImVec2(25.0f, 25.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(0.0f, 400.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Toolbox"))
	{
		ImGui::BeginChild("Available Object", ImVec2(150.0f, 200.0f), true);
		for (int i = 0; i < 9; ++i)
		{
			if (m_Entities[i])
			{
				char label[10];
				sprintf(label, "Object %d", i + 1);
				if (ImGui::Selectable(label, m_SelectedEntityIdx == i))
				{
					m_SelectedEntityIdx = i;
				}
			}
		}
		ImGui::EndChild();
		if (m_SelectedEntityIdx != -1 && m_Entities[m_SelectedEntityIdx])
		{
			if (ImGui::Button("Delete"))
			{
				m_Registry.destroy(*m_Entities[m_SelectedEntityIdx]);
				m_Entities[m_SelectedEntityIdx].reset();
				m_SelectedEntityIdx = -1;
				m_SystemRender.Update(m_Registry);
			}
		}

		int renderMethod = m_GUIRenderMethod;
		ImGui::RadioButton("Painter's Algorithm", &renderMethod, (int)BaseRenderSystem::RenderMethod::Painter);
		ImGui::RadioButton("ZBuffer", &renderMethod, (int)BaseRenderSystem::RenderMethod::ZBuffer);
		if (m_GUIRenderMethod != renderMethod)
		{
			m_GUIRenderMethod = renderMethod;

			m_SystemRender.ChangeRenderMethod(m_Registry, (BaseRenderSystem::RenderMethod)m_GUIRenderMethod);
		}

		ImGui::InputFloat("Step", &m_GUITransformStep, 10.0f, 10.0f);
		if (m_GUITransformStep < 5.0f)
		{
			m_GUITransformStep = 5.0f;
		}

		int transformationMethod = m_GUITransformationMethod;
		ImGui::RadioButton("Translation", &transformationMethod, (int)TransformationMethod::Translation);
		ImGui::RadioButton("Rotation", &transformationMethod, (int)TransformationMethod::Rotation);
		if (m_GUITransformationMethod != transformationMethod)
		{
			m_GUITransformationMethod = transformationMethod;
		}

		if (m_SelectedEntityIdx != -1 && m_Entities[m_SelectedEntityIdx])
		{
			if (transformationMethod == (int)TransformationMethod::Rotation)
			{
				if (ImGui::Button("X+"))
				{
					glm::dvec3 vec(0.0);
					vec.x = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * 1.0
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("X-"))
				{
					glm::dvec3 vec(0.0);
					vec.x = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * -1.0
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Y+"))
				{
					glm::dvec3 vec(0.0);
					vec.y = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * 1.0
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Y-"))
				{
					glm::dvec3 vec(0.0);
					vec.y = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * -1.0
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Z+"))
				{
					glm::dvec3 vec(0.0);
					vec.z = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * 1.0
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Z-"))
				{
					glm::dvec3 vec(0.0);
					vec.z = 1.0;
					m_SystemTransform.Rotate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec,
						10.0 * -1.0
					);
					m_SystemRender.Update(m_Registry);
				}
			}
			else if (transformationMethod == (int)TransformationMethod::Translation)
			{
				if (ImGui::Button("Forward"))
				{
					glm::dvec3 vec(0.0);
					vec.z -= 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Backward"))
				{
					glm::dvec3 vec(0.0);
					vec.z += 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Up"))
				{
					glm::dvec3 vec(0.0);
					vec.y += 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Down"))
				{
					glm::dvec3 vec(0.0);
					vec.y -= 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Right"))
				{
					glm::dvec3 vec(0.0);
					vec.x += 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
				if (ImGui::Button("Left"))
				{
					glm::dvec3 vec(0.0);
					vec.x -= 10.0;
					m_SystemTransform.Translate(
						m_Registry,
						*m_Entities[m_SelectedEntityIdx],
						vec
					);
					m_SystemRender.Update(m_Registry);
				}
			}
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(425.0f, 25.0f), ImGuiCond_FirstUseEver);
	if (m_GUIWindowTips)
	{
		if (ImGui::Begin("Tips", &m_GUIWindowTips))
		{
			ImGui::Text("Press F1 to change the transformation mode to translation");
			ImGui::Text("Press F2 to change the transformation mode to rotation");
			ImGui::Text("Press F3 to change the rendering algorithm to Painter's Algorithm");
			ImGui::Text("Press F4 to change the rendering algorithm to Z-Buffer");
			ImGui::Text("Press 1 to 9 to change the selected object");
			ImGui::Text("Press ESC to deselect the selected object");
			ImGui::Text("Press Q, W, E, A, S, D to do the transformation");
			ImGui::Text("Press P when using Painter Algorithm to print the tree");
		}
		ImGui::End();
	}

	if (m_GUIShowMessageBox)
	{
		ImGui::SetNextWindowPos(ImVec2(300.0f, 200.0f));
		if (ImGui::Begin("INFO", &m_GUIShowMessageBox,
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove     |
			ImGuiWindowFlags_NoResize))
		{
			ImGui::Text(m_GUIMessageText.c_str());
		}
		ImGui::End();
	}
}

void App::Update()
{
	try
	{
		m_SystemRender.Update(m_Registry);
	}
	catch (std::exception& ex)
	{
		ShowMessageBox(std::string("Error when trying to update the data.\n") + ex.what());
	}
}

void App::ShowMessageBox(std::string& messageText)
{
	m_GUIMessageText = messageText;
	m_GUIShowMessageBox = true;
}

void App::Render()
{
	m_SystemRender.Render(m_Registry, m_Window);
	ImGui::SFML::Render(m_Window);
	m_Window.display();
}
