#pragma once

#include "Scene.h"

class Avalon : public Scene
{
public:
	Avalon(std::string name);

	virtual void InitScene(float windowWidth, float windowHeight) override;

	int m_Player;
	int Player();


};