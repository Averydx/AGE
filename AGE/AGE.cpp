// AGE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
using namespace AGE; 
int main()
{
    Engine app{ 800, 600, "App" };
    app.Run(); 
    glfwTerminate();
    return 0;

    

}

