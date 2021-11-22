#pragma once
#include <SFML/Graphics.hpp>
using namespace sf; using namespace std;
class Picture {
public:				//� SFML ������ � �������� ������������ ����� ��������:Image, Sprite, Texture. ��� �������� ���������� �� � ����
	Image image;
	Texture texture;//���� ��������
	Sprite sprite;//���� ������
	float X = 0, Y = 0;

	Picture() {};
	Picture(string File_name, float x, float y)
	{
		image.loadFromFile("images/" + File_name + ".png");
		image.createMaskFromColor(Color(245,72,255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);//�������� ������ ���������
		X = x; Y = y;
		sprite.setPosition(X, Y);
	}
};