#include "Menu.h"
#include "Game.h"
#include "Avalon.h"
#include "EntityIdentifier.h"


Menu::Menu(std::string name)
	: Scene(name)
{
}



void Menu::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//Camera Entity
	{
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		ECS::AttachComponent<Camera>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Camera");
		ECS::SetIsMainCamera(entity, true);
	}

	{
		auto entityBack = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entityBack);
		ECS::AttachComponent<Transform>(entityBack);

		//Sets up components
		std::string BackGround = "MenuPlay.png"; //400, 200
		ECS::GetComponent<Sprite>(entityBack).LoadSprite(BackGround, 380, 200);
		ECS::GetComponent<Transform>(entityBack).SetPosition(vec3(0.f, 0.f, 13.f));

		//Setup up the Identifier
		unsigned int bitHolder = 0x0;
		ECS::SetUpIdentifier(entityBack, bitHolder, "Temp Entity");
	}
	

	{

		
		MenuButton ButtonID;
		auto mapLayout = File::LoadJSON("MenuScreen1.json");

		
		
		//Creates entity
		auto globalMap = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(globalMap);
		ECS::AttachComponent<Transform>(globalMap);
		ECS::AttachComponent<AnimationController>(globalMap);


		//Set up components
		std::string globalMapFile = "WholeMenu.png";
		auto& menuAnim = ECS::GetComponent<AnimationController>(globalMap);
		menuAnim.InitUVs(globalMapFile);

		//Adds first animation
		menuAnim.AddAnimation(mapLayout["PlayMenu"]);	// 0		//0
		menuAnim.AddAnimation(mapLayout["ControlsMenu"]);	// 1	//1
		menuAnim.AddAnimation(mapLayout["CreditsMenu"]);	// 2	//2
	

		//Sets active animation
		menuAnim.SetActiveAnim(0);
		//Gets first animation

		ECS::GetComponent<Sprite>(globalMap).LoadSprite(globalMapFile, 380, 200, true, &menuAnim);
		ECS::GetComponent<Sprite>(globalMap).SetUVs(vec2(14.f, 34.f), vec2(30.f, 11.f));
		ECS::GetComponent<Transform>(globalMap).SetPosition(vec3(0.f, 0.f, 13.f));

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(globalMap, bitHolder, "Global Map");
		

		m_mainMenu = globalMap;
	}

	
	
}

int Menu::Menu1()
{
	return m_mainMenu;
}