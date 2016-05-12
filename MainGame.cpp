#include "MainGame.h"

#include <GamaGameEngine/GamaGameEngine.h>
#include <iostream>


MainGame::MainGame() :
	m_screenWidth(800),
	m_screenHeight(454),
	m_gameState(GameState::PLAY),
	m_fps(0),
	m_maxFPS(60.0f),
	m_player(nullptr)
{
	m_camera.init(m_screenWidth, m_screenHeight);
}


MainGame::~MainGame() {
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	for (unsigned int i = 0; i < m_npcs.size(); i++) {
		delete m_npcs[i];
	}
	for (unsigned int i = 0; i < m_players.size(); i++) {
		delete m_players[i];
	}
}

void MainGame::run() {

	GamaGameEngine::AudioEngine AE;
	initSystems();
	GamaGameEngine::MusicStream ogg;

	char* fileName = "Audio/Level.ogg";
	ogg.init(fileName);
	ogg.attachBufferToSource();
	ogg.play();

	gameLoop();
}

void MainGame::initSystems() {

	GamaGameEngine::init();

	m_window.create("DemoGame", m_screenWidth, m_screenHeight, 0);

	initLevel();

	initShaders();

	m_levelSpriteBatch.init();
	m_characterSpriteBatch.init();

	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	m_fpsLimiter.setMaxFPS(m_maxFPS);

}

void MainGame::initLevel() {

	Level* level = new Level();
	level->init("Levels/Round1.png", m_world);
	m_levels.push_back(level);
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(m_world, 3.0f, m_levels[m_currentLevel]->getStartPlayerPos(), glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT), &m_camera, 30);
	m_players.push_back(m_player);

	for (int i = 500; i < 7000; i = i + 500 ) {
		m_npcs.push_back(new NPC);
		m_npcs.back()->init(m_world, 3.0f, glm::vec2(float(i), -100.0f), glm::vec2(NPC_WIDTH, NPC_HEIGHT), 100);
	}
}

void MainGame::initShaders() {
	m_colourProgram.compileShaders("Shaders/colourShading.vert", "Shaders/colourShading.frag");
	m_colourProgram.addAttribute("vertexPosition");
	m_colourProgram.addAttribute("vertexColour");
	m_colourProgram.addAttribute("vertexUV");
	m_colourProgram.linkShaders();
}



void MainGame::gameLoop() {
	
	// Main game loop
	while (m_gameState == GameState::PLAY) {

		m_fpsLimiter.begin();

		m_world->step();

		processInput();

		m_camera.update();

		checkForCollisions();

		//Add delta time to prevent spiral of death
		updateCharacters();

		drawLevel();

		//m_inputManager.update();

		m_fps = m_fpsLimiter.limitFPS();

		//print only every 100 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 100) {
			std::cout << m_fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::updateCharacters() {
	for (unsigned int i = 0; i < m_players.size(); i++) {
		m_players[i]->update(m_inputManager, *m_world);
	}

	for (unsigned int i = 0; i < m_npcs.size(); i++) {
		m_npcs[i]->update();
	}
}

void MainGame::processInput() {

	SDL_Event evnt;

	const float CAMERA_SPEED = 3.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
		m_player->getCentrePos();
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;

		direction = glm::normalize(direction);
	}
}

void MainGame::drawLevel() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colourProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = m_colourProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint pLocation = m_colourProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_levels[m_currentLevel]->draw();

	m_characterSpriteBatch.begin(GamaGameEngine::SpriteSortType::BACK_TO_FRONT);

	const glm::vec2 agentDims(50.0f * 2.0f);

	for (unsigned int i = 0; i < m_players.size(); i++) {
		if (m_camera.isBoxInView(m_players[i]->getCentrePos(), agentDims)) {
			m_players[i]->draw(m_characterSpriteBatch);
		}
	}

	float test = m_player->getCentrePos().x;

	for (unsigned int i = 0; i < m_npcs.size(); i++) {
		if (m_camera.isBoxInView(m_npcs[i]->getCentrePos(), agentDims)) {
			m_npcs[i]->draw(m_characterSpriteBatch, test);
		}
	}

	//Draws collision bodies for debug purposes
	//m_world->draw();

	m_characterSpriteBatch.end();

	m_characterSpriteBatch.renderBatch();

	m_window.swapBuffer();
}


void MainGame::checkForCollisions() {

	// Generate new collision info
	m_contacts.clear();
	for (unsigned int i = 0; i < m_players.size(); i++) {
		GamaGameEngine::RigidBody* A = m_players[i]->getAttackBox();

		for (unsigned int j = 0; j < m_npcs.size(); j++) {
			GamaGameEngine::RigidBody* B = m_npcs[j]->getAttackBox();

			GamaGameEngine::Manifold m(A, B);
			m.getCollisionInfo();

			//is conatact made - is facing centre of other - is hitting
			if (m.contactCount) {

				if (m_players[i]->getAnimState() == AnimationState::PUNCHING) {
					m_npcs[j]->setAnimState(AnimationState::HIT);
					//remove hard coded damage value
					m_npcs[j]->setHealth(10);
				}
				m_collision = true;
			}
		}
	}
}
