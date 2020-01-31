#include "Avalon.h"


Avalon::Avalon(std::string name)
	: Scene(name)
{
}



void Avalon::InitScene(float windowWidth, float windowHeight)
{
	
	m_sceneReg = new entt::registry;
	ECS::AttachRegister(m_sceneReg);
	float aspectRatio = windowWidth / windowHeight;
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
		std::string BackGround = "TestMap.png"; //400, 200
		ECS::GetComponent<Sprite>(entityBack).LoadSprite(BackGround, 380, 200);
		ECS::GetComponent<Transform>(entityBack).SetPosition(vec3(0.f, 0.f, 13.f));

		//Setup up the Identifier
		unsigned int bitHolder = 0x0;
		ECS::SetUpIdentifier(entityBack, bitHolder, "Temp Entity");
	}
	


	{
		//Our attack animation file
		auto Moving = File::LoadJSON("Square1.json");
		//auto Moving = File::LoadJSON("Main_Character1.json");

		//Creates entity
		auto Player1 = ECS::CreateEntity();

		//Main Player
		EntityIdentifier::MainPlayer(Player1);

		//Add components
		ECS::AttachComponent<Sprite>(Player1);
		ECS::AttachComponent<Transform>(Player1);
		ECS::AttachComponent<AnimationController>(Player1);

		//Sets up components
		//std::string SpaceRanger = "Space Rager 2.png";
		std::string SpaceRanger = "Green.png";
		//std::string SpaceRanger = "Main_Character.png";
		auto& animControllerr = ECS::GetComponent<AnimationController>(Player1);
		animControllerr.InitUVs(SpaceRanger);

		//Adds first Animation
		//animControllerr.AddAnimation(Animation());
										//Calling the name of the animation

		animControllerr.AddAnimation(Moving["Anim1"]);


		//Set first anitmation
		animControllerr.SetActiveAnim(0);

		//gets first animation
		auto& anim = animControllerr.GetAnimation(0);

		//Makes it repeat
		anim.SetRepeating(true);

		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f);
		//485 547
		ECS::GetComponent<Sprite>(Player1).LoadSprite(SpaceRanger, 10, 10, true, &animControllerr);
		ECS::GetComponent<Sprite>(Player1).SetUVs(vec2(14.f, 34.f), vec2(30.f, 11.f));
		//30, -25, 20
		ECS::GetComponent<Transform>(Player1).SetPosition(vec3(0.f, 0.f, 15.f));

		//Setup up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(Player1, bitHolder, "Green Entity");

		ECS::SetIsMainPlayer(Player1, true);

		m_Player = Player1;

	}

}



int Avalon::Player()
{
	return m_Player;
}

