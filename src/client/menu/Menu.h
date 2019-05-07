#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_ITEMS 3

class Menu
{
public:
	Menu(float width, float height, sf::RenderWindow & window);
	~Menu();

	void update();
	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }

private:

	sf::RenderWindow& _window;
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];

};

