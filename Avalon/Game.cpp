#include "Game.h"
#include "Menu.h"
#include <random>

int scenenum = 1;

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

//What happens when game starts
void Game::InitGame()
{
	//Scene names and clear colors
	m_name = "Fury of Death";
	m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	//Initializes the backend
	BackEnd::InitBackEnd(m_name);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	m_scenes.push_back(new Menu("Menu Screen"));
	m_scenes.push_back(new Avalon("Avalon"));
	

	m_activeScene = m_scenes[scenenum];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	if (scenenum == 0)
	{
		CreateMenuButton();
	}

	m_register = m_activeScene->GetScene();
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with clearColor
		m_window->Clear(m_clearColor);
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();
		
		//Flips the windows
		m_window->Flip();
		
		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	//Just calls all the other input functions 
	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::KeyboardHold()
{
	//Keyboard button held
	auto& animControllerr = ECS::GetComponent<AnimationController>(2);
	vec3 position = m_register->get<Transform>(EntityIdentifier::MainPlayer()).GetPosition();

	float speed = 30.f;
	//Keyboard button held
	if (Input::GetKey(Key::W)) {
		//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(++position.y + (speed * Timer::deltaTime));
		m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionY((position.y + 0.25f) + (speed * Timer::deltaTime));

	}
	if (Input::GetKey(Key::S)) {
		//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(--position.y - (speed * Timer::deltaTime));
		m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionY((position.y - 0.25f) - (speed * Timer::deltaTime));
		

	}
	if (Input::GetKey(Key::A)) {
		//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(--position.x - (speed * Timer::deltaTime));
		m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX((position.x - 0.25f) - (speed * Timer::deltaTime));

	}
	if (Input::GetKey(Key::D)) {
		//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
		m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX((position.x + 0.25f) + (speed * Timer::deltaTime));


	}
}

void Game::KeyboardDown()
{
	//Keyboard button down
}

void Game::KeyboardUp()
{
	if (Input::GetKeyUp(Key::F1))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}



void Game::CreateMenuButton()
{

	if (Input::GetKey(Key::UpArrow) && m_activeScene == 0) {
	//change imagecount to +1
		Menu* scene = (Menu*)m_activeScene;
		auto menuEntity = scene->Menu1();
		auto& globalMap = ECS::GetComponent<AnimationController>(menuEntity);
		globalMap.SetActiveAnim(1);
		auto& anim = globalMap.GetAnimation(1);
	}
	if (Input::GetKey(Key::DownArrow) && m_activeScene == 0) {
	//change imagecount to -1
		Menu* scene = (Menu*)m_activeScene;
		auto menuEntity = scene->Menu1();
		auto& globalMap = ECS::GetComponent<AnimationController>(menuEntity);
		globalMap.SetActiveAnim(2);
		auto& anim = globalMap.GetAnimation(2);
	}

	
	
}