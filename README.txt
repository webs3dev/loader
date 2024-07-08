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


README.txt
-----------
this whole source code is based off of https://github.com/ocornut/imgui and using its core libary so if you want to know about it just go over at https://github.com/ocornut/imgui/wiki and read , id also recommend compiling this in visual stuido , since it will be faster and easier prob could read at https://www.reddit.com/r/Cplusplus/comments/w32xj1/how_to_compile_c_code_on_vscode/


references:
-----------

- https://github.com/ocornut/imgui
- https://github.com/ocornut/imgui/wiki

