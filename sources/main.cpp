#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include <iostream>
#include <cmath>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;

	std::shared_ptr<Shader> phong = ResourceManager::getInstance().getShader(0);
	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 0.0f, 1.0f, 1.f));
	PointLight & light2 = lights.createPointLight(Color(1.0f, 0.0f, 0.0f, 1.f), Vector3(2.f, 1.f, 1.f));
	light.rotateY(40.f);

	Model model("resources/Trex/TrexByJoel3d.fbx");

	glfwSetTime(0.f);
	float lastTime = 0.f;
	float dt = 0.f;
	float frameLimit = 1.f / 300.f;
	float timer = 0.f;
	while (win.isOpen())
	{
		// Compute frametime
		float currentTime = glfwGetTime();
		float frametime = (currentTime - lastTime);
		lastTime = currentTime;

		dt += frametime;
		if (dt < frameLimit)
			continue ;

		timer += dt;
		// Inputs
		if (Keyboard::isKeyPressed(GLFW_KEY_ESCAPE))
			win.close();

		// Update (first lights and view)
		lights.update();
		camera.update(dt);

		model.rotateY(dt * 40.f);
		light2.translate(Vector3(0.05f, 0.f, 0.f) * std::cos(timer));
		//light.rotateX(10.f * dt);
		//light.rotateZ(50.f * dt);
		//light.rotateY(-50.f * dt);

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		win.clear();
		phong->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		phong->setParameter("ViewMatrix", camera.getViewMatrix());
		phong->setParameter("view_position", camera.getPosition());
		model.draw();

		win.display();
		win.pollEvents();
	}
	return (0);
}
