#pragma once
#include <SFML/Graphics.hpp>
using namespace sf; using namespace std;
class Picture {
public:				//в SFML работа с графикой организуется тремя классами:Image, Sprite, Texture. для удобства объединили их в один
	Image image;
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт
	float X = 0, Y = 0;

	Picture() {};
	Picture(string File_name, float x, float y)
	{
		image.loadFromFile("images/" + File_name + ".png");
		image.createMaskFromColor(Color(245,72,255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);//заливаем спрайт текстурой
		X = x; Y = y;
		sprite.setPosition(X, Y);
	}
};