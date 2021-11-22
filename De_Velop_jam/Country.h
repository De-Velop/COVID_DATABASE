#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <Picture.h>
#include <vector>
using namespace sf; using namespace std;
class Country :public Picture
{								//класс "Страна" описывает всевозможные данные о стране
public:
	struct record //запись из датасета
	{
		unsigned day=0, month=0, year=0, cases=0, deaths=0;
	};
	IntRect intrect;
	string name;
	unsigned long long int population, cases_total = 0, deaths_total = 0;
	float density;
	vector<record> covid_data;
	Picture *flag;

	Country(string country_name, unsigned long long int x, unsigned long long int y, unsigned long long int n, float p) :Picture("strany/" + country_name, x, y)
	{
		flag = new Picture("flags/" + country_name, 50, 50);
		image.createMaskFromColor(Color(34, 177, 76), 0);
		name = country_name;
		population = n;
		density = p;
		intrect.left = x; intrect.top = y; intrect.width = texture.getSize().x; intrect.height = texture.getSize().y;
		sprite.setColor(Color(0, 0, 0, 0));
	};

	bool isSpritePressed(int MX, int MY) //проверка нажатия на страну
	{
		if (intrect.contains(MX, MY))
		{
			if (image.getPixel(MX - X, MY - Y) == Color::White)
			{
				return true;
			}
			else return false;
		}
		else return false;
	};

	void animation(float time) // анимация подсветки
	{
		sprite.setColor(Color(255, 255, 255, (Uint8)(-20 * cos(4 * time) + 20)));
	};

	void setSpriteColor()
	{
		sprite.setColor(Color(255, 255, 255, 0));
	};

	void push_record(unsigned day, unsigned month, unsigned year, unsigned cases, unsigned deaths) //добавление записи в массив (вектор)
	{
		covid_data.push_back({day,month,year,cases,deaths});
	}
};

