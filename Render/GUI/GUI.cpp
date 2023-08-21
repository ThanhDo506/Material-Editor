#include "GUI.h"
#include <iostream>
#include "Time.h"
#include "../../World Entities/CameraController.h"
#include "../../World Entities/Camera.h"
#include "../../third-party/ImGuiFileDialog-master/ImGuiFileDialog.h"
// #include "../Object/Model.h"
#include "../TextureMap.h"
#include <string>

#include "../../utilities/utilities.h"
#include "../Lighting/Light.h"

GUI::GUI()
{
    p_application = nullptr;
}

void GUI::textureModelHandle(TextureMap* texture)
{
    std::string type = TextureMap::to_string(texture->getType());
    ImGui::SeparatorText(type.c_str());
    ImGui::Image((void*)static_cast<intptr_t>(texture->_texture._id), ImVec2(128, 128));
    ImGui::SameLine();
    if (ImGui::Button(type.c_str()))
        ImGuiFileDialog::Instance()->OpenDialog(type, "Choose " + type, ".png,.jpg", ".");
    // display
    if (ImGuiFileDialog::Instance()->Display(type)) 
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            //texture->loadTexture(filePathName, texture->_type, GL_UNSIGNED_BYTE, false);
            Texture newTexture;
            newTexture.load(filePathName.c_str());
            texture->clean();
            delete texture;
            texture = new TextureMap(newTexture,texture->getType());
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

GUI& GUI::instance()
{
    static GUI instance;
    return instance;
}

void GUI::init(Application* application, const char* version)
{
    this->p_application = application;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // set Font
    io.Fonts->AddFontFromFileTTF("Resources/fonts/Roboto-Medium.ttf", 16.0f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(application->p_glfwWindow, true);
    ImGui_ImplOpenGL3_Init(version);
}

void GUI::draw()
{
    static ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    {
        static bool vsync = false;
        if (ImGui::Begin("Setting"))
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            if (ImGui::Checkbox("Vsync", &vsync))
            {
                this->p_application->setVsync(vsync);
            }
            if(ImGui::TreeNode("Other setting"))
            {
                ImGui::ShowStyleEditor();
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
    
    ImGui::Begin("Scene Setting");
    ImVec2 sceneSettingSize = ImGui::GetItemRectSize();
    
    if (ImGui::TreeNode("View Camera"))
    {
        static auto camera = WorldManager::instance().p_mainCamera;
        ImGui::DragFloat3("Position",&camera->transform.position[0]);
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(camera->transform.rotation));
        if (ImGui::DragFloat3("Rotation", &rotation[0]))
        {
            camera->transform.rotation = Transform::eulerAnglesToQuaternion(rotation);
        }
        if (ImGui::TreeNode("Advanced Setting"))
        {
            ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
            ImGui::DragFloat("Near", &camera->_near, 1, 0.001, 1000, "%.3f");
            ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
            ImGui::SameLine();
            ImGui::DragFloat("Far", &camera->_far, 10, 1000, 10000, "%.f");
            ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
            ImGui::SameLine();
            ImGui::DragFloat("Fov", &camera->_zoom, 1, 0, 175, "%.f");
            
            ImGui::SeparatorText("Camera Movement");
            static auto camController = WorldManager::instance().p_mainCameraController;
            ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
            ImGui::DragFloat("Speed", &camController->_speed);
            ImGui::DragFloat2("Sensitivity", &camController->_sensitivityX);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    
    if (ImGui::TreeNode("Hierarchy"))
    {
        for (auto i : WorldManager::instance()._entities)
        {
            if (ImGui::TreeNode("Transform"))
            {
                static glm::vec3 rotation = i->transform.eulerAngles();
                ImGui::DragFloat3("Position", &i->transform.position[0]);
                if (ImGui::DragFloat3("Rotation", &rotation[0]))
                {
                    i->transform.rotation = Transform::eulerAnglesToQuaternion(rotation);
                }
                ImGui::DragFloat3("Scale", &i->transform.scale[0]);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    
    if (ImGui::TreeNode("Light Manager"))
    {
        static auto lightManager = WorldManager::instance().p_lightManager;
        
        if(ImGui::TreeNode("Directional Lights"))
        {
            for (auto light : lightManager->_lights)
            {
                if(light->getLightType() == Light::LightType::DIRECTIONAL_LIGHT)
                {
                    auto dirLight = dynamic_cast<DirectionalLight*>(light);
                    if(ImGui::TreeNode(std::string("Directional light #" + std::to_string( light->getLightIndex())).c_str()))
                    {
                        ImGui::SeparatorText("Index");
                        ImGui::DragFloat3("Direction", &dirLight->_direction[0]);
                        ImGui::ColorEdit3("Color", &dirLight->_ambient[0]);
                        ImGui::DragFloat3("Diffuse", &dirLight->_diffuse[0]);
                        ImGui::DragFloat3("Specular", &dirLight->_specular[0]);
                        if(ImGui::Button("Remove light"))
                        {
                            lightManager->removeLight(dirLight);
                        }
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::NewLine();
            if(ImGui::Button("Add new Light"))
            {
                lightManager->addLight(new DirectionalLight);
            }
            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Point Lights"))
        {
            for (auto light : lightManager->_lights)
            {
                if(light->getLightType() == Light::LightType::POINT_LIGHT)
                {
                    auto pointLight = dynamic_cast<PointLight*>(light);
                    std::string lightIndex = std::to_string( light->getLightIndex());
                    if (ImGui::TreeNode(std::string("Point light #" + lightIndex).c_str()))
                    {
                        ImGui::ColorEdit3("Color", &pointLight->_ambient[0]);
                        ImGui::DragFloat3("Diffuse", &pointLight->_diffuse[0]);
                        ImGui::DragFloat3("Specular", &pointLight->_specular[0]);

                        ImGui::SeparatorText("Attenuation");
                        Light::Attenuation *attenuation = &pointLight->_attenuation;
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::DragFloat("Constant", &attenuation->constant, 0.25, 0.001, 10, "%.3f");
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::SameLine();
                        ImGui::DragFloat("Linear", &attenuation->linear, 0.25, 0.001, 10, "%.3f");
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::SameLine();
                        ImGui::DragFloat("Quadratic", &attenuation->quadratic, 0.25, 0.001, 10, "%.3f");

                        if(ImGui::Button("Remove light"))
                        {
                            lightManager->removeLight(pointLight);
                        }
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::NewLine();
            if(ImGui::Button("Add new Light"))
            {
                lightManager->addLight(new PointLight);
            }
            ImGui::TreePop();
        }
        
        if(ImGui::TreeNode("Spot Lights"))
        {
            for (auto light : lightManager->_lights)
            {
                if(light->getLightType() == Light::LightType::SPOT_LIGHT)
                {
                    auto spotLight = dynamic_cast<SpotLight*>(light);
                    std::string lightIndex = std::to_string( spotLight->getLightIndex());
                    if (ImGui::TreeNode(std::string("Spot light #" + lightIndex).c_str()))
                    {
                        ImGui::DragFloat3("Position", &spotLight->_position[0]);
                        ImGui::DragFloat3("Direction", &spotLight->_direction[0]);
                        ImGui::ColorEdit3("Color", &spotLight->_ambient[0]);
                        ImGui::DragFloat3("Diffuse", &spotLight->_diffuse[0]);
                        ImGui::DragFloat3("Specular", &spotLight->_specular[0]);

                        ImGui::DragFloat("Inner", &spotLight->_innerAngle);
                        ImGui::DragFloat("Outer", &spotLight->_outerAngle);

                        ImGui::SeparatorText("Attenuation");
                        Light::Attenuation *attenuation = &spotLight->_attenuation;
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::DragFloat("Constant", &attenuation->constant, 0.25, 0.001, 10, "%.3f");
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::SameLine();
                        ImGui::DragFloat("Linear", &attenuation->linear, 0.25, 0.001, 10, "%.3f");
                        ImGui::SetNextItemWidth(sceneSettingSize.x * 0.15);
                        ImGui::SameLine();
                        ImGui::DragFloat("Quadratic", &attenuation->quadratic, 0.25, 0.001, 10, "%.3f");

                        if(ImGui::Button("Remove light"))
                        {
                            lightManager->removeLight(spotLight);
                        }
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::NewLine();
            if(ImGui::Button("Add new Spot light"))
            {
                lightManager->addLight(new SpotLight);
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

	if (ImGui::TreeNode("Shader Manager"))
	{
	    static auto shaderManager = WorldManager::instance().p_shaderManager;
		if (shaderManager != nullptr) {
			ImGui::Text("Total shader %d", shaderManager->shaderMaps.size());
			if(ImGui::BeginTable("Shader Table", 2))
			{
				for (auto shader : shaderManager->shaderMaps)
				{
					/*ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Name %s", shader.first);
					ImGui::TableNextColumn();
					ImGui::Text("Id %s", shader.second->getId());*/
				}
				ImGui::EndTable();
			}
		}
	    ImGui::TreePop();
	}
    if (ImGui::CollapsingHeader("Post Processing"))
    {
        if (ImGui::TreeNode("HDR Setting"))
        {
            static const char* items[]     = {"None", "Reinhard", "Exposure"};
            static int         currentItem = 0;
            // TODO: make Frame Buffer to a class and clean these trashes :(
            if (ImGui::Combo("HDR", &currentItem, items, IM_ARRAYSIZE(items)))
            {
                switch (currentItem)
                {
                case 0:
                    p_application->hdr = 0;
                    break;
                case 1:
                    p_application->hdr = 1;
                    break;
                case 2:
                    p_application->hdr = 2;
                    break;
                default: break;
                }
            }
            if(currentItem == 2)
                ImGui::SliderFloat("exposure", &p_application->exposure, 0.002, 5.0f);
            ImGui::TreePop();
        }
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    AppLog("Cleaning ImGUI");
}
