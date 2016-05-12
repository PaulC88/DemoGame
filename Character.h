#pragma once

#include <GamaGameEngine/SpriteBatch.h>
#include <GamaGameEngine/SpriteSheet.h>
#include <GamaGameEngine/ResourceManager.h>
#include <GamaGameEngine/PhysicsWorld.h>

#include "BoundingBox.h"


enum class AnimationState { STANDING, WALKING, PUNCHING, KICKING, JUMPING, HIT, DEAD };

class NPC;
class Player;

class Character {

public:
	Character();
	virtual ~Character();
	
	GamaGameEngine::RigidBody* addBoundingBox(GamaGameEngine::Shape* shape, int x, int y);

	void draw(GamaGameEngine::SpriteBatch& m_spriteBatch);

	void setCollisionBox();

	glm::vec2 getCentrePos();
	glm::vec2 getPosition() const { return m_position;  }
	GamaGameEngine::RigidBody* getAttackBox() const { return m_attackBox; }
	AnimationState getAnimState() const { return m_animState; }
	bool isNPC() const { return m_isNPC; }

	void setPosition(glm::vec2 pos) { m_position = pos; }
	void setHealth(int damage) { m_health -= damage; };
	

	void getBodies(std::vector<GamaGameEngine::RigidBody*> &bodies);


protected:
	BoundingBox m_box;

	glm::vec2 m_spriteDimensions;
	glm::vec2 m_position;
	GamaGameEngine::Colour m_colour;
	GamaGameEngine::SpriteSheet m_texture;
	float m_speed; 
	int m_health;
	GLuint m_textureID;

	bool m_isNPC = false;
	bool m_isPunching = false;
	bool m_isWalking = false;
	bool m_isJumping = false;
	bool m_isKicking = false;
	bool m_isHit = false;
	bool m_facingRight = true;
	
	GamaGameEngine::RigidBody* m_attackBox;

	AnimationState m_animState = AnimationState::STANDING;
	float m_animationTime = 0.0f;
	std::vector<GamaGameEngine::RigidBody*> m_charBodies;
};

