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

class reTime;
class reInput;

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
	static reTime* GetTime();
	static reInput* GetInput();

public:
	static reEngine::Impl* m_instance;
};