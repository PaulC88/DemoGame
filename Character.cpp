#include "Character.h"


Character::Character() {
	//Empty
}


Character::~Character() {
	//Empty
}

void Character::draw(GamaGameEngine::SpriteBatch& m_spriteBatch) {
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = m_spriteDimensions.x;
	destRect.w = m_spriteDimensions.y;

	m_spriteBatch.addSprite(destRect, uvRect, m_textureID, 0.0f, m_colour);
}

glm::vec2 Character::getCentrePos() {
	glm::vec2 pos = getPosition();
	pos.x = pos.x + (m_spriteDimensions.x / 2);
	pos.y = pos.y + (m_spriteDimensions.y / 2);
	return pos;
}

GamaGameEngine::RigidBody* Character::addBoundingBox(GamaGameEngine::Shape* shape, int x, int y) {
	assert(shape);
	GamaGameEngine::RigidBody *b = new GamaGameEngine::RigidBody(shape, x, y);
	m_charBodies.push_back(b);
	return b;
}

void Character::getBodies(std::vector<GamaGameEngine::RigidBody*> &bodies) {
	for (unsigned int i = 0; i < m_charBodies.size(); i++) {
		bodies.emplace_back(m_charBodies[i]);
	}
}

void Character::setCollisionBox() {
	GamaGameEngine::Polygon poly;
	poly.setBox(30.0f, 10.0f);
	glm::vec2 pos = getCentrePos();
	if (m_isNPC == true)
		pos.y += 20;
	m_attackBox = addBoundingBox(&poly, (int)pos.x, (int)pos.y);
	m_attackBox->setOrient(0);

	m_attackBox->setMass(0.9f);
	m_attackBox->setRestitution(0.2f);
	m_attackBox->setDynFriction(0.2f);
	m_attackBox->setStatFriction(0.4f);
}