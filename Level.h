#pragma once

#include <GamaGameEngine/SpriteBatch.h>
#include <GamaGameEngine/DataTypes.h>
#include <GamaGameEngine/ResourceManager.h>
#include <GamaGameEngine/Shape.h>
#include <GamaGameEngine/Manifold.h>
#include <GamaGameEngine/PhysicsWorld.h>

#include "BoundingBox.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Level {

public:
	Level();
	~Level();

	void init(const std::string filePath,GamaGameEngine::PhysicsWorld* world);
	void draw();
	GamaGameEngine::RigidBody* addBorder(GamaGameEngine::Shape *shape, int x, int y);
	void drawBorders();

	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }

	void getBorders(std::vector<GamaGameEngine::RigidBody*> &bodies);

private:

	BoundingBox m_border;
	int m_iterations;
	GamaGameEngine::SpriteBatch m_spriteBatch;
	std::vector<GamaGameEngine::RigidBody*> m_borders;
	std::vector<GamaGameEngine::Manifold> m_contacts;
	glm::vec2 m_startPlayerPos;

};

