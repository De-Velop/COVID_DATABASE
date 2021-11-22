#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Camera
{
			//класс камера отвечает за перемещение экрана по карте и отображении спрайтов в нужных позициях
public:
	View view ;
	float wheel = 1;
	float Cx, Cy;
	IntRect Left, Right, Top, Bottom;
	int speed = 13;

	Camera(Vector2f mapSize, Vector2i winSize)
	{
		view = View(Vector2f(mapSize.x / 2, mapSize.y / 2), Vector2f(mapSize.x, mapSize.y));
		Left = IntRect(0, 0, 13, winSize.y), Top = IntRect(0, 0, winSize.x, 13);
		Right = IntRect(winSize.x - 13, 0, 13, winSize.y), Bottom = IntRect(0, winSize.y - 13, winSize.x, 13);
		Cx = mapSize.x / 2;
		Cy = mapSize.y / 2;
	};

	void scale(float delta) //отвечает за масштаб
	{
		wheel *= 1 - delta * 0.1;

		view.zoom(1 - delta * 0.1);
		if (wheel > 1)
		{
			view.zoom(1 / wheel);
			wheel = 1;
		}
		if (wheel < 0.5f)
		{
			view.zoom(0.5f / wheel);
			wheel = 0.5f;
		}
	};

	void move(Vector2i mPos) //перемещение камеры
	{
		if (Left.contains(mPos))Cx -= speed;
		if (Top.contains(mPos))Cy -= speed;
		if (Right.contains(mPos))Cx += speed;
		if (Bottom.contains(mPos))Cy += speed;

		if (Cx - 1440 * wheel - 5 < 0)Cx = 0 + 1440 * wheel;
		if (Cy - 900 * wheel - 3 < 0)Cy = 0 + 900 * wheel;
		if (Cx + 1440 * wheel + 5 > 2880)Cx = 2880 - 1440 * wheel;
		if (Cy + 900 * wheel + 3 > 1800)Cy = 1800 - 900 * wheel;

		view.setCenter(Cx, Cy);
	};
};

