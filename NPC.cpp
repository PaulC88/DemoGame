#include "NPC.h"



NPC::NPC() {
	//empty
}


NPC::~NPC() {
	//empty
}

void NPC::init(GamaGameEngine::PhysicsWorld* world, float speed, glm::vec2 pos,
	glm::vec2 spriteDimensions, int health) {

	m_isNPC = true;
	GamaGameEngine::GLTexture texture = GamaGameEngine::ResourceManager::getTexture("Sprites/Galsia.png");
	m_speed = speed;
	m_spriteDimensions = spriteDimensions;
	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;
	m_health = health;
	m_texture.init(texture, glm::ivec2(4, 5));
	m_box.init(world, pos, glm::vec2(35.0f, 70.0f), 1.0f, true);
	m_position = m_box.getPosition() - (m_spriteDimensions / 2.0f);
	setCollisionBox();
}

void NPC::draw(GamaGameEngine::SpriteBatch& spriteBatch, float playerPos) {

	int spriteIndex;
	int spriteNum;

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = m_spriteDimensions.x;
	destRect.w = m_spriteDimensions.y;

	float animationSpeed = 0.15f;

	if (m_health <= 0) {
		spriteNum = 1;
		spriteIndex = 8;
		if (m_colour.a != 0) {
			m_colour.a -= 1;
		}
	}
	else if (m_isHit) {
		spriteNum = 1;
		spriteIndex = 12;
		if (m_animState != AnimationState::HIT) {
			m_animState = AnimationState::HIT;
			m_animationTime = 0.0f;
		}
	}
	else if (m_isPunching) {
		spriteNum = 1;
		spriteIndex = 1;
		//animationSpeed *= 0.25f;
		if (m_animState != AnimationState::PUNCHING) {
			m_animState = AnimationState::PUNCHING;
			m_animationTime = 0.0f;
		}
	}
	else {
		//Standing Still
		spriteNum = 3;
		spriteIndex = 16;
		m_animState = AnimationState::STANDING;
	}

	m_animationTime += animationSpeed;

	if (m_animationTime > spriteNum) {
		m_isPunching = false;
		m_isWalking = false;
		m_isJumping = false;
		m_isKicking = false;
		m_isHit = false;
		m_animState = AnimationState::WALKING;
	}
	spriteIndex = spriteIndex + (int)m_animationTime % spriteNum;

	glm::vec4 uvRect = m_texture.getUVs(spriteIndex);

	//Flip the sprite to face left if player is facing left
	if (!m_facingRight) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	//Draw collision box for debugging purposes
	//for (unsigned int i = 0; i < m_charBodies.size(); ++i) {
	//	GamaGameEngine::RigidBody *b = m_charBodies[i];
	//	b->shape->draw();
	//}

	if (playerPos < getCentrePos().x) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	//Plus 20.0f necessary for sprite accuracy - not using in-house sprites
	spriteBatch.addSprite(destRect, uvRect, m_texture.texture.id, (getCentrePos().y + 20.0f), m_colour);
}

void NPC::update() {

	if (m_health == 0) {
		AnimationState::DEAD;
	}
	else {
		if (m_animState == AnimationState::HIT) {
			m_isHit = true;
		}
	}
}

void NPC::setAnimState(AnimationState animState) {
	m_animState = animState;
}