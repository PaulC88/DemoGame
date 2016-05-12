#include "Level.h"
#include <GamaGameEngine/GamaMath.h>



Level::Level() {

}

Level::~Level() {

}

void Level::init(const std::string filePath, GamaGameEngine::PhysicsWorld* world) {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_startPlayerPos.x = -100;
	m_startPlayerPos.y = -100;

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 posAndSize(-420.0f, -226.0f, 7688, 384);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GamaGameEngine::GLTexture texture = GamaGameEngine::ResourceManager::getTexture(filePath);
	GamaGameEngine::Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;
	m_border.init(world, glm::vec2(3700.0f, 30.0f),glm::vec2(7400.0f, 5.0f), 0.0f, false);
	m_border.m_body->setStatic();
	m_border.init(world, glm::vec2(3700.0f, -220.0f), glm::vec2(7400.0f, 5.0f), 0.0f, false);
	m_border.m_body->setStatic();
	m_border.init(world, glm::vec2(-400.0f, 0.0f), glm::vec2(5.0f, 500.0f), 0.0f, false);
	m_border.m_body->setStatic();
	m_border.init(world, glm::vec2(7265.0f, 0.0f), glm::vec2(5.0f, 500.0f), 0.0f, false);
	m_border.m_body->setStatic();

	m_spriteBatch.addSprite(posAndSize, uv, texture.id, 0.0f, colour);

	m_spriteBatch.end();

	draw();

	//glBindTexture(GL_TEXTURE_2D, 0);

	//m_window.swapBuffer();
}

void Level::drawBorders(void) {
	for (unsigned int i = 0; i < m_borders.size(); ++i)
	{
		GamaGameEngine::RigidBody *b = m_borders[i];
		b->shape->draw();
	}

}

void Level::draw() {
	m_spriteBatch.renderBatch();
	for (unsigned int i = 0; i < m_borders.size(); ++i)
	{
		GamaGameEngine::RigidBody *b = m_borders[i];
		b->shape->draw();
	}
	drawBorders();
}

GamaGameEngine::RigidBody* Level::addBorder(GamaGameEngine::Shape *shape, int x, int y) {
	assert(shape);
	GamaGameEngine::RigidBody *b = new GamaGameEngine::RigidBody(shape, x, y);
	m_borders.push_back(b);
	return b;
}

void Level::getBorders(std::vector<GamaGameEngine::RigidBody*> &bodies) {
	for (unsigned int i = 0; i < m_borders.size(); i++) {
		bodies.emplace_back(m_borders[i]);
	}
}