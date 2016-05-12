#include "Player.h"

#include <GamaGameEngine/GamaMath.h>
#include <GamaGameEngine/Shape.h>
#include <GamaGameEngine/RigidBody.h>
#include <GamaGameEngine/Manifold.h>
#include <SDL/SDL.h>


Player::Player() {
	//empty
}


Player::~Player() {
	//empty
}

void Player::init(GamaGameEngine::PhysicsWorld* world, float speed, glm::vec2 pos, 
		glm::vec2 spriteDimensions, GamaGameEngine::Camera* camera, int health) {

	GamaGameEngine::GLTexture texture = GamaGameEngine::ResourceManager::getTexture("Sprites/Axel.png");
	m_speed = speed;
	m_spriteDimensions = spriteDimensions;
	m_camera = camera;
	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;
	m_health = health;
	m_texture.init(texture, glm::ivec2(9, 11));
	m_box.init(world, pos, glm::vec2(35.0f, 70.0f), 1.0f, true);
	m_playerID = world->bodies.size() - 1;
	m_position = m_box.getPosition() - (m_spriteDimensions / 2.0f);
	setCollisionBox();
	se.init("Audio/Slap.wav");
	se.attachBufferToSource();
}

void Player::draw(GamaGameEngine::SpriteBatch& spriteBatch) {

	int spriteIndex;
	int spriteNum;
	glm::vec4 destRect;

	GamaGameEngine::RigidBody* body = m_box.getBody();
	destRect.x = body->getPosition().x - m_spriteDimensions.x / 2.0f;
	destRect.y = body->getPosition().y - m_spriteDimensions.y / 2.0f;
	destRect.z = m_spriteDimensions.x;
	destRect.w = m_spriteDimensions.y;

	float animationSpeed = 0.15f;

	if (m_isPunching) {
		spriteNum = 1;
		spriteIndex = 48;
		if (m_animState != AnimationState::PUNCHING) {
			m_animState = AnimationState::PUNCHING;
			m_animationTime = 0.0f;
		}
	}
	else if (m_isWalking) { 
		spriteNum = 6;
		spriteIndex = 93;
			if (m_animState != AnimationState::WALKING) {
				m_animState = AnimationState::WALKING;
				m_animationTime = 0.0f;
			}
	}
	else if (m_isKicking) {
		spriteNum = 3;
		spriteIndex = 51;
		if (m_animState != AnimationState::KICKING) {
			m_animState = AnimationState::KICKING;
			m_animationTime = 0.0f;
		}
	}
	else {
		spriteNum = 3;
		spriteIndex = 90;
		m_animState = AnimationState::STANDING;
	}


	//else if (m_isJumping) {
	//	spriteNum = 3;
	//	spriteIndex = 81;
	//	if (m_animState != AnimationState::JUMPING) {
	//		m_animState = AnimationState::JUMPING;
	//		m_animationTime = 0.0f;
	//	}
	//}
	//else if (m_isKicking) {
	//	//animationSpeed *= 2.0f;
	//	spriteNum = 3;
	//	spriteIndex = 51;
	//	if (m_animState != AnimationState::KICKING) {
	//		m_animState = AnimationState::KICKING;
	//		m_animationTime = 0.0f;
	//	}
	//}

	m_animationTime += animationSpeed;

	if (m_animationTime > spriteNum) {
		m_isPunching = false;
		m_isWalking = false;
		m_isJumping = false;
		m_isKicking = false;
	}
	spriteIndex = spriteIndex + (int)m_animationTime % spriteNum;

	glm::vec4 uvRect = m_texture.getUVs(spriteIndex);

	//Flip the sprite to face left if player is facing left
	if (!m_facingRight) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	//Draw the players attack BB for debugging purposes
	//for (unsigned int i = 0; i < m_charBodies.size(); ++i) {
	//	GamaGameEngine::RigidBody *b = m_charBodies[i];
	//	b->shape->draw();
	//}

	spriteBatch.addSprite(destRect, uvRect, m_texture.texture.id, getCentrePos().y, m_colour);
}

void Player::update(GamaGameEngine::InputManager& m_inputManager, GamaGameEngine::PhysicsWorld& world) {

	setPosition(glm::vec2(world.bodies[m_playerID]->getPosition().x - (m_spriteDimensions.x / 2.0f), 
			world.bodies[m_playerID]->getPosition().y - (m_spriteDimensions.y / 2.0f)));

	GamaGameEngine::RigidBody* body = m_box.getBody();

	//makes box follow player
	for (unsigned int i = 0; i < m_charBodies.size(); i++) {
		m_charBodies[i]->setPosition(GamaGameEngine::vec2(getCentrePos().x, getCentrePos().y));
	}

	if (m_inputManager.isKeyPressed(SDLK_j)) {
		m_isPunching = true;
		se.play();
		m_inputManager.releaseKey(SDLK_j);
	}
	else if (m_inputManager.isKeyPressed(SDLK_k)) {
		m_isKicking = true;
	}
	else if (m_inputManager.isKeyPressed(SDLK_l)) {
		m_isJumping = true;
	}
	else if (m_inputManager.isKeyHeld(SDLK_w)) {
		body->applyForce(GamaGameEngine::vec2(0.0f, 100.0f));
 
		world.bodies[m_playerID]->setYPosition(world.bodies[m_playerID]->getPosition().y + m_speed);
		m_isWalking = true;
	}
	else if (m_inputManager.isKeyHeld(SDLK_s) && m_position.y) { 
		body->applyForce(GamaGameEngine::vec2(0.0f, -100.0f));

		world.bodies[m_playerID]->setYPosition(world.bodies[m_playerID]->getPosition().y - m_speed);
		m_isWalking = true;
	}
	if (m_inputManager.isKeyHeld(SDLK_a)) {
		body->applyForce(GamaGameEngine::vec2(-100.0f, 0.0f));

		if (m_position.x > 0.0f && m_position.x < 6850.0f)
			m_camera->setPosition(m_camera->getPosition() + glm::vec2(-3.0f, 0.0f));
		
		world.bodies[m_playerID]->setXPosition(world.bodies[m_playerID]->getPosition().x - m_speed);
		m_isWalking = true;
		m_facingRight = false;
	}
	else if (m_inputManager.isKeyHeld(SDLK_d)) {
		body->applyForce(GamaGameEngine::vec2(100.0f, 0.0f));

		if (m_position.x > 0.0f && m_position.x < 6850.0f)
			m_camera->setPosition(m_camera->getPosition() + glm::vec2(3.0f, 0.0f));
			
		world.bodies[m_playerID]->setXPosition(world.bodies[m_playerID]->getPosition().x + m_speed);
		m_isWalking = true;
		m_facingRight = true;
	}
}
