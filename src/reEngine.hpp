#pragma once

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

namespace reGraphics
{
	class reShaderManager;
	class reModelManager;
	class reMaterialManager;
	class reRenderer;
}

namespace rePhysics
{
	class rePhysicsSystem;
}

class reTime;
class reInput;
class reDebugRender;

class reEngine
{
public:
	class Impl;

	reEngine() = delete;

	static reEngine::Impl* GetInstance();
	static void SetInstance(reEngine::Impl* instance);

	static reGraphics::reShaderManager* GetShaderManager();
	static reGraphics::reModelManager* GetModelManager();
	static reGraphics::reMaterialManager* GetMaterialManager();
	static reGraphics::reRenderer* GetRenderer();
	static rePhysics::rePhysicsSystem* GetPhysicsSystem();
	static reTime* GetTime();
	static reInput* GetInput();
	static reDebugRender* GetDebugRender();

public:
	static reEngine::Impl* m_instance;
};