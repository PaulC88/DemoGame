#pragma once

#include "Character.h"


//Measurements taken from Spritesheet
const float NPC_WIDTH = 98.0f;
const float NPC_HEIGHT = 144.0f;

class NPC : public Character {

public:
	NPC();
	~NPC();

	void init(GamaGameEngine::PhysicsWorld* world, float speed, glm::vec2 position,
		glm::vec2 spriteDimensions, int health);

	void draw(GamaGameEngine::SpriteBatch& spriteBatch, float playerPos);

	void update();

	void setAnimState(AnimationState animState);

private:
	
};

