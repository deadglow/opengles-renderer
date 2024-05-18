#include "reTestListener.hpp"

#include "reCamera.hpp"
#include "reEngine.hpp"
#include "reModelManager.hpp"
#include "reRenderer.hpp"
#include "reModel.hpp"
#include "reTime.hpp"
#include "reInput.hpp"
#include "reShaderManager.hpp"

using namespace reGraphics;

void TempInputStuff(reFlyCam& camera)
{
	reInput* input = reEngine::GetInput();
	reTime* time = reEngine::GetTime();
	if (input->reloadShader)
		reEngine::GetShaderManager()->ReloadAllShaders();

	vec2_t camInput = vec2(input->camXAxis.Value(), input->camYAxis.Value());
	vec3_t moveInput = vec3(input->moveXAxis.Value(), input->moveYAxis.Value(), input->moveZAxis.Value());

	camera.MoveWithInput(camInput, moveInput, time->deltaTime);
}

void TempModelStuff(reFlyCam& camera)
{
	auto* renderer = reEngine::GetRenderer();
	renderer->m_camera = &camera.m_camera;
	
	auto* modelManager = reEngine::GetModelManager();
	auto modelGuid = *modelManager->GetModelIDByName("monkie");

	reTime* time = reEngine::GetTime();

	reTransform t = reTransform::Identity();
	rotor3_t r = rot3_plane_angle(bivector3(1.f, 0.f, 0.f), (float)time->now * 1.f);
	t.Rotate(r);

	//renderer->AddModelToRender(reModelInst(modelGuid), t.ConstructMatrix());
}

void reTestListener::OnUpdate()
{
}

void reTestListener::OnPreRender()
{
	TempInputStuff(m_flyCam);
	TempModelStuff(m_flyCam);
}

void reTestListener::OnRegister()
{
	// TEMP
	auto* modelManager = reEngine::GetModelManager();
	auto& model = modelManager->LoadModel("monkie");
	modelManager->GPULoad(model.m_guid);
}
