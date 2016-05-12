#pragma once

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <memory>

#include <GamaGameEngine/PhysicsWorld.h>
#include <GamaGameEngine/Window.h>
#include <GamaGameEngine/InputManager.h>
#include <GamaGameEngine/Camera.h>
#include <GamaGameEngine/SpriteBatch.h>
#include <GamaGameEngine/DataTypes.h>
#include <GamaGameEngine/ResourceManager.h>
#include <GamaGameEngine/RigidBody.h>
#include <GamaGameEngine/FrameRateManager.h>
#include <GamaGameEngine/GLSLProgram.h>
#include <GamaGameEngine/AudioEngine.h>
#include <GamaGameEngine/MusicStream.h>
#include <GamaGameEngine/Shape.h>
#include <GamaGameEngine/InputManager.h>
#include <GamaGameEngine/Clock.h>
#include <GamaGameEngine/Manifold.h>
#include <GamaGameEngine/GamaMath.h>
#include "BoundingBox.h"
#include <algorithm>

#include "Player.h"
#include "NPC.h"
#include "Level.h"

enum class GameState {
	PLAY,
	EXIT
};

class MainGame {

public:
	MainGame();
	~MainGame();

	void run();

	void checkForCollisions();

	bool m_collision = false;

private:
	
	void initSystems();
	void initShaders();
	void initLevel();
	void gameLoop();
	void updateCharacters();
	void processInput();
	void drawLevel();
	void drawGame();

	GamaGameEngine::PhysicsWorld* m_world = new GamaGameEngine::PhysicsWorld(0.0f, 10);

	GamaGameEngine::Window m_window;
	GamaGameEngine::GLSLProgram m_textureProgram;
	GamaGameEngine::InputManager m_inputManager;
	GamaGameEngine::Camera m_camera;
	GamaGameEngine::Camera m_hudCamera;
	GamaGameEngine::SpriteBatch m_levelSpriteBatch;
	GamaGameEngine::SpriteBatch m_characterSpriteBatch;
	GamaGameEngine::GLSLProgram m_colourProgram;
	GamaGameEngine::FpsLimiter m_fpsLimiter;
	GamaGameEngine::Clock g_Clock;

	Player* m_player;
	NPC* npc;

	std::vector<Player*> m_players;
	std::vector<NPC*> m_npcs;
	std::vector<Level*> m_levels;
	std::vector<GamaGameEngine::Manifold> m_contacts;

	int m_iterations;
	int m_currentLevel;
	int m_screenWidth, m_screenHeight;
	float m_fps, m_maxFPS;
	float m_dt;

	GameState m_gameState;
};

