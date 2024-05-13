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

void TempInputStuff(reCamera& camera)
{
	reInput* input = reEngine::GetInput();
	reTime* time = reEngine::GetTime();
	if (input->reloadShader)
		reEngine::GetShaderManager()->ReloadAllShaders();

	vec2_t camInput = vec2(input->camXAxis.Value(), input->camYAxis.Value());
	vec3_t moveInput = vec3(input->moveXAxis.Value(), input->moveYAxis.Value(), input->moveZAxis.Value());

	float angle = 70;
	angle *= M_DEG2RAD * time->deltaTime;
	float speed = 10;
	speed *= time->deltaTime;

	camera.m_viewAngles.x += angle * camInput.y;
	camera.m_viewAngles.y += angle * -camInput.x;
	camera.ApplyViewAngles();

	reTransform& camTransform = camera.m_transform;

	vec3_t deltaXZ = v3_add(v3_muls(v3_right, moveInput.x), v3_muls(v3_forward, moveInput.z));
	deltaXZ = v3_muls(v3_norm(deltaXZ), speed);
	deltaXZ = rot3_transform(camTransform.rotation, deltaXZ);
	vec3_t deltaUp = v3_muls(v3_up, moveInput.y * speed);
	vec3_t translation = v3_add(deltaXZ, deltaUp);
	camTransform.position = v3_add(camTransform.position, translation);
}

void TempModelStuff(reCamera& camera)
{
	auto* renderer = reEngine::GetRenderer();
	renderer->m_camera = &camera;
	
	auto* modelManager = reEngine::GetModelManager();
	auto modelGuid = *modelManager->GetModelIDByName("monkie");

	reTime* time = reEngine::GetTime();

	reTransform t = reTransform::Identity();
	rotor3_t r = rot3_plane_angle(bivector3(1.f, 0.f, 0.f), (float)time->time * 1.f);
	t.Rotate(r);

	renderer->AddModelToRender(reModelInst(modelGuid), t.ConstructMatrix());
}

void reTestListener::OnUpdate()
{
}

void reTestListener::OnPreRender()
{
	TempInputStuff(m_camera);
	TempModelStuff(m_camera);
}

void reTestListener::OnRegister()
{
	// TEMP
	auto* modelManager = reEngine::GetModelManager();
	auto& model = modelManager->LoadModel("monkie");
	modelManager->GPULoad(model.m_guid);
}
