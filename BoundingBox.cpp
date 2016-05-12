#include "BoundingBox.h"


void BoundingBox::init(GamaGameEngine::PhysicsWorld* world, const glm::vec2& position, 
		const glm::vec2& dimensions, float mass, bool ignore) {

	m_dimensions = dimensions;
	
	GamaGameEngine::Polygon poly;
	m_position = position;
	poly.setBox(dimensions.x, dimensions.y);
	m_body = world->add(&poly, (int)position.x, (int)position.y);
	m_body->setOrient(0);

	if (ignore)
		m_body->setIgnoreCase();
}

void BoundingBox::draw() {
	m_body->shape->draw();
}

void BoundingBox::update() {

}
