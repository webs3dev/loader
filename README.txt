simple csgo loader that looks really cool

example cpp 
----------------------
#include "imgui.h"

void RenderImGui()
{
    ImGui::Begin("CS:GO Loader");

    if (ImGui::Button("Enable Aimbot"))
    {
        /* exmpty */
    }

    ImGui::Checkbox("ESP", &espEnabled);

    ImGui::End();
}

//main loop
void MainLoop()
{
    while (gameIsRunning)
    {
       

        
        ImGui_ImplXXX_NewFrame(); // Replace XXX with your platform/renderer

        RenderImGui();

        // resent frame, etc.
    }
}



references:
-----------

- [ImGui GitHub Repository](https://github.com/ocornut/imgui)
- [ImGui Documentation](https://github.com/ocornut/imgui/wiki)
