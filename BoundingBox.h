#pragma once

#include <glm/glm.hpp>

#include <GamaGameEngine/PhysicsWorld.h>
#include <GamaGameEngine/RigidBody.h>
#include <GamaGameEngine/Shape.h>

class BoundingBox {

public:

	void init(GamaGameEngine::PhysicsWorld* world, const glm::vec2& position, 
			const glm::vec2& dimensions, float mass, bool ignore);

	void draw();

	void update();

	//Getters
	GamaGameEngine::RigidBody* getBody() const { return m_body; }
	const glm::vec2& getDimensions() const { return m_dimensions; }
	const glm::vec2& getPosition() const { return m_position; }

	//Setters
	void setPosition(glm::vec2 pos) { m_position = pos; }
	void setYPos(float posY) { m_position.y = posY; }

	GamaGameEngine::RigidBody* m_body = nullptr;

private:
	
	bool m_ignore;
	glm::vec2 m_dimensions;
	glm::vec2 m_position;
};

