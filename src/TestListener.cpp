#include "TestListener.hpp"

#include "Camera.hpp"
#include "Engine.hpp"
#include "ModelManager.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "ShaderManager.hpp"

using namespace Enoki;
using namespace Enoki::Graphics;

void FlyCamStuff(FlyCam& camera)
{
	Time* time = Engine::GetTime();
	Input* input = Engine::GetInput();

	vec2_t camInput = vec2(input->camXAxis.Value(), input->camYAxis.Value());
	vec3_t moveInput = vec3(input->moveXAxis.Value(), input->moveYAxis.Value(), input->moveZAxis.Value());

	camera.MoveWithInput(camInput, moveInput, time->deltaTime.count());
}

void CharacterControllerStuff(PlayerController& playerController)
{
	Input* input = Engine::GetInput();
}

void CharacterControllerCameraStuff(PlayerController& playerController)
{
	Input* input = Engine::GetInput();
	vec2_t camInput = vec2(-input->mouseAxes.y, -input->mouseAxes.x);
	
	playerController.UpdateViewAndCamera({ camInput });

	Time* time = Engine::GetTime();
	float dt = time->deltaTime.count();
	playerController.MoveWithInput({ vec2(input->moveXAxis.Value(), input->moveZAxis.Value()) }, dt);
}

void TempGraphicsStuff()
{
	Time* time = Engine::GetTime();
	
	Input* input = Engine::GetInput();
	if (input->reloadShader)
		Engine::GetShaderManager()->ReloadAllShaders();

	Transform t = Transform::Identity();
	rotor3_t r = rot3_plane_angle(bivector3(1.f, 0.f, 0.f), (float)time->now.count() * 1.f);
	t.Rotate(r);

	auto* renderer = Engine::GetRenderer();
	auto* modelManager = Engine::GetModelManager();

	auto modelGuid = *modelManager->GetModelIDByName("monkie");
	renderer->AddModelToRender(ModelInst(modelGuid), t.ConstructMatrix());

	auto levelGuid = *modelManager->GetModelIDByName("level");
	Transform t2 = Transform::Identity();
	// rotor3_t r2 = rot3_plane_angle(bivector3(0.f, 1.f, 0.f), 180.f * M_DEG2RAD);
	// t2.Rotate(r2);
	renderer->AddModelToRender(ModelInst(levelGuid), Transform::Identity().ConstructMatrix());
}

void TestListener::OnUpdate()
{
	CharacterControllerStuff(playerController);
}

void TestListener::OnPreRender()
{
	CharacterControllerCameraStuff(playerController);

	TempGraphicsStuff();

	auto* renderer = Engine::GetRenderer();
	renderer->camera = &playerController.camera;
}

void TestListener::OnRegister()
{
	// TEMP
	auto* modelManager = Engine::GetModelManager();
	auto& model = modelManager->LoadModel("monkie");
	modelManager->GPULoad(model.guid);

	auto& levelModel = modelManager->LoadModel("level");
	modelManager->GPULoad(levelModel.guid);
}
