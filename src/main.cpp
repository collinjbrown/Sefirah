// main.cpp
//

#include <iostream>
#include "engine.h"

int main(void)
{
	Sefirah::Engine* engine = new Sefirah::Engine(1800, 800, "Sefirah", 500, 64);

	engine->Update();
}