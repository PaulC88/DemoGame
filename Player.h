#pragma once

#include "Character.h"

#include <GamaGameEngine/InputManager.h>
#include <GamaGameEngine/Camera.h>
#include <GamaGameEngine/Manifold.h>
#include <GamaGameEngine/Shape.h>
#include <GamaGameEngine/SoundEffects.h>

#include <vector>

//Measurements taken from Spritesheet
const float PLAYER_WIDTH = 128.0f; 
const float PLAYER_HEIGHT = 224.0f;


class Player : public Character {

public:
	Player();
	~Player();

	void init(GamaGameEngine::PhysicsWorld* world, float speed, glm::vec2 position, 
			glm::vec2 spriteDimensions, GamaGameEngine::Camera* camera, int health);

	void draw(GamaGameEngine::SpriteBatch& spriteBatch);

	void update(GamaGameEngine::InputManager& inputManager, GamaGameEngine::PhysicsWorld& world);

	const BoundingBox& getBox() const { return m_box; }	

private:

	unsigned int m_playerID;
	
	GamaGameEngine::Camera* m_camera;
	GamaGameEngine::SoundEffects se;
	std::vector<GamaGameEngine::Manifold> m_contacts;
	
};

