#pragma once

#include "math_3d.h"
#include "reEngine.hpp"
#include "reShaderManager.hpp"
#include "reTime.hpp"
#include "reCamera.hpp"

#include "gles2.h"

namespace reGraphics
{
	static void SetSharedUniforms(const reCamera& camera)
	{
		auto shaderManager = reEngine::GetShaderManager();
		auto time = reEngine::GetTime();

		float t = time->time;
		shaderManager->SetUniform("u_time", &t);

		float aspect = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		mat4_t perspective = camera.GetPerspectiveMatrix(aspect);
		mat4_t iPerspective = m4_invert_affine(perspective);
		mat4_t iViewMat = camera.GetTransform();
		mat4_t viewMat = m4_invert_affine(iViewMat);

		mat4_t vpMat = m4_mul(perspective, viewMat);
		mat4_t ivpMat = m4_invert_affine(vpMat);

		shaderManager->SetUniform("u_pMat", &perspective);
		shaderManager->SetUniform("u_ipMat", &iPerspective);
		shaderManager->SetUniform("u_vMat", &viewMat);
		shaderManager->SetUniform("u_ivMat", &iViewMat);
	}

	static void DoRenderStuff(SDL_Window* window, const reCamera& camera)
	{
		auto shaderManager = reEngine::GetShaderManager();
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SetSharedUniforms(camera);

		shaderManager->UseShader(*shaderManager->GetCurrentShader());

		// loop through draw requests...
		
		// Draw a triangle from the 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}
	
}