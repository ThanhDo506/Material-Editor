#include "GUI.h"
#include <iostream>
#include "Time.h"
#include "../../World Entities/CameraController.h"
#include "../../World Entities/Camera.h"
#include "../../third-party/ImGuiFileDialog-master/ImGuiFileDialog.h"
// #include "../Object/Model.h"
#include "../TextureMap.h"
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
        // Create the combo box
        static const char* items[]     = {"Dark Mode", "Light Mode", "Classic Mode"};
        static int         currentItem = 0;

        static bool vsync = false;
        if (ImGui::Begin("Setting"))
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            if (ImGui::Combo("##combo", &currentItem, items, IM_ARRAYSIZE(items)))
            {
                switch (currentItem)
                {
                case 0: ImGui::StyleColorsDark();
                    break;
                case 1: ImGui::StyleColorsLight();
                    break;
                case 2: ImGui::StyleColorsClassic();
                    break;
                default: break;
                }
            }
            if (ImGui::Checkbox("Vsync", &vsync))
            {
                this->p_application->setVsync(vsync);
            }
            ImGui::ShowStyleEditor();
        }
        ImGui::End();
    }
#pragma region Hierarchy
    ImGui::Begin("Hierarchy");
    //if (ImGui::CollapsingHeader("Hierarchy")) {
    for (auto i : WorldManager::instance()._entities)
    {
        if (ImGui::TreeNode(i->_name.c_str()))
        {
            if (i->_name == "Main Camera")
            {
                Camera* mainCam = dynamic_cast<Camera*>(i);
                glm::vec3 eulerAngler = mainCam->transform.eulerAngles();
                ImGui::Checkbox("Active", &mainCam->_isActive);
                ImGui::SeparatorText("Transform");
                ImGui::InputFloat3("Position", &mainCam->transform.position[0]);
                // Euler angles
                ImGui::SetNextItemWidth(82.0f);
                if(ImGui::DragFloat("Pitch", &eulerAngler.x, 0.05f, -360.0f, 360.0f))
                {
                    mainCam->transform.rotation = Transform::eulerAnglesToQuaternion(eulerAngler);
                }
                ImGui::SetNextItemWidth(82.0f);
                ImGui::SameLine();
                if(ImGui::DragFloat("Yaw", &eulerAngler.y, 0.05f, -360.0f, 360.0f))
                {
                    mainCam->transform.rotation = Transform::eulerAnglesToQuaternion(eulerAngler);
                }
                ImGui::SetNextItemWidth(82.0f);
                ImGui::SameLine();
                if(ImGui::DragFloat("Roll", &eulerAngler.z, 0.05f, -360.0f, 360.0f))
                {
                    mainCam->transform.rotation = Transform::eulerAnglesToQuaternion(eulerAngler);
                }
                ImGui::SeparatorText("Advance setting");
                // Near far
                ImGui::SetNextItemWidth(82.0f);
                ImGui::DragFloat("Near", &mainCam->_near, 0.05f, 0.001f, 10000.0f);
                ImGui::SetNextItemWidth(82.0f);
                ImGui::SameLine();
                ImGui::DragFloat("Far", &mainCam->_far, 0.05f, 0.001f, 10000.0f);
                // Screen
                ImGui::SetNextItemWidth(82.0f);
                ImGui::DragInt("Screen Width", &mainCam->_width, 1, 640, 1920);
                ImGui::SetNextItemWidth(82.0f);
                ImGui::SameLine();
                ImGui::DragInt("Screen Height", &mainCam->_height, 1, 480, 1080);
            } else if (i->_name == "Movement Controller")
            {
                static auto movementController = dynamic_cast<CameraController*>(i);
                ImGui::SliderFloat("Speed", &movementController->_speed, 0.02, 20.0f);
                ImGui::SliderFloat2("Sensitivity", &movementController->_sensitivityX, 0.1, 1500.0);
                ImGui::Checkbox("Is forcusing", &movementController->_isForcus);
                ImGui::Text("delta Mouse: X %3.3f Y %3.3f", Input::instance().getDeltaMousePosition().x, Input::instance().getDeltaMousePosition().y);
            } else if (i->_name == "Model")
            {
                // auto model = dynamic_cast<Model *>(i);
                // Material *mat = &model->_material;
                // this->textureModelHandle(&mat->_albedo);
                // this->textureModelHandle(&mat->_metallic);
                // this->textureModelHandle(&mat->_normal);
                // this->textureModelHandle(&mat->_roughness);
                // this->textureModelHandle(&mat->_ao);
                // this->textureModelHandle(&mat->_emission);
            }

            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNode("HDR"))
    {
        static const char* items[]     = {"None", "Reinhard", "Exposure"};
        static int         currentItem = 0;
        if (ImGui::Combo("##combo", &currentItem, items, IM_ARRAYSIZE(items)))
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
        
        ImGui::SliderFloat("exposure", &p_application->exposure, 0.002, 20.0f);
        ImGui::TreePop();
    }

    
    ImGui::End();
#pragma endregion
    

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
