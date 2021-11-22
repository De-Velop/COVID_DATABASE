#pragma once
#include <SFML/Graphics.hpp>
#include <Picture.h>
#include <Country.h>
#include <sstream>
#include <iomanip>
using namespace sf; using namespace std;
class Menu :public Picture						//класс "ћеню" отвечает за вывод текстовой информации на экран
{
public:
	RenderTexture rTexture, rendGraph;
	Sprite result,spGraph;
	Text* text = new Text;
	Font font;
	RectangleShape rect;
	Picture* pictGraph = new Picture("graph",0,0);
	Picture* point = new Picture("point", 0, 0);
	Picture* appName = new Picture("appName",0,0);

	Menu() :Picture("Menu", 0, 0)
	{
		rTexture.create(texture.getSize().x, texture.getSize().y);
		font.loadFromFile("LemonMilkRegular-X3XE2.otf");
		text->setFont(font);
		rect.setSize(Vector2f(235, 235));
		rect.setPosition(247, 464);
		rect.setFillColor(Color(153, 217, 234));
		rendGraph.create(pictGraph->texture.getSize().x, pictGraph->texture.getSize().y);
	};
	
	string separate(string s) {									//добавление раздел€ющих точек дл€ удобного представлени€ целых чисел
		for (int i = s.length() - 1, count = 0; i > 0; i--)
		{
			count++;
			if (count == 3) {
				count = 0;
				s.insert(i, ",");
			}
		}
		return s;
	}

	void makeLabel(Text* text, string s, unsigned char_size, int x, int y, bool border) //создание надписи с нужными свойствами
	{
		text->setCharacterSize(char_size);
		text->setString(s);
		if(!border)text->setOrigin(text->getLocalBounds().width / 2, text->getLocalBounds().height / 2);
		else text->setOrigin(0, text->getLocalBounds().height / 2);
		text->setPosition(x, y);
	}

	Sprite drawAppName(float Cx, float Cy, float wheel, Vector2f mapSize) {		//надпись "Covid-19 simulation" в главном окне
		appName->sprite.setOrigin(appName->sprite.getLocalBounds().width/2,0);
		appName->sprite.setScale(wheel,wheel);
		appName->sprite.setPosition(Cx,Cy-mapSize.y/2*wheel);
		return  appName->sprite;
	}

	Sprite drawGraph(Country* c, float Cx, float Cy, float wheel, Vector2f mapSize) {	//рисует графики дл€ стран по данным из датасета
		rendGraph.clear(Color::Transparent);
		rendGraph.draw(pictGraph->sprite);
		unsigned long long temp_case=0,temp_death=0;
		for (int i = 0; i < c->covid_data.size(); i++) {
			temp_case += c->covid_data[i].cases;
			temp_death += c->covid_data[i].deaths;
			point->sprite.setColor(Color(255,156,108));
			point->sprite.setPosition((float)i/ (float)c->covid_data.size()*1040.f+100, 900- (float)temp_case/ (float)c->cases_total*640.f);
			rendGraph.draw(point->sprite);
			point->sprite.setColor(Color(0, 0, 0));
			point->sprite.setPosition(point->sprite.getPosition().x, 900 - (float)temp_death / (float)c->cases_total * 640.f);
			rendGraph.draw(point->sprite);
		}
		makeLabel(text, to_string(c->cases_total), 27, 55, 259, 1);
		text->setFillColor(Color::Black);
		rendGraph.draw(*text);

		rendGraph.display();
		spGraph.setTexture(rendGraph.getTexture());
		spGraph.setOrigin(spGraph.getLocalBounds().width/2, spGraph.getLocalBounds().height/2);
		spGraph.setPosition(Cx - 100 * wheel , Cy);

		spGraph.setScale(wheel, wheel);

		return spGraph;
	}
	Sprite drawMenu(Country* c, float Cx, float Cy, float wheel,Vector2f mapSize,bool btn) //выводит всю инфрмацию о стране в поле справа
	{
		text->setFillColor(Color::White);

		rTexture.clear(Color::Transparent);
		c->flag->sprite.setPosition(45,45);
		c->flag->sprite.setScale(0.75f, 0.75f);
		rTexture.draw(c->flag->sprite);

		
		rTexture.draw(rect);

		sf::ConvexShape c_shape;
		c_shape.setPointCount(3);
		c_shape.setFillColor(Color(240, 228, 176));
		c_shape.setPoint(0, sf::Vector2f(362, 580));
		c_shape.setPoint(1, sf::Vector2f(362, 464));
		c_shape.setPoint(2, sf::Vector2f(364+(float(c->cases_total) / float(c->population)*2000), 464));
		rTexture.draw(c_shape);

		sf::ConvexShape d_shape;
		d_shape.setPointCount(3);
		d_shape.setFillColor(Color(242, 91, 99));
		d_shape.setPoint(0, sf::Vector2f(361, 580));
		d_shape.setPoint(1, sf::Vector2f(359 - (float(c->deaths_total) / float(c->population) * 4000), 464));
		d_shape.setPoint(2, sf::Vector2f(361, 464));
		rTexture.draw(d_shape);

		rTexture.draw(sprite);

		makeLabel(text, c->name, 27, 375, 100,0);
		rTexture.draw(*text);

		makeLabel(text, "Population", 27, 135, 250,0);
		rTexture.draw(*text);

		string temp_s = to_string(c->population);

		makeLabel(text, separate(temp_s), 27, 375, 250,0);
		rTexture.draw(*text);

		makeLabel(text, "Density", 27, 135, 300,0);
		rTexture.draw(*text);

		stringstream ss;
		ss << fixed << setprecision(1) << c->density;
		temp_s = ss.str();
		makeLabel(text, temp_s, 27, 375, 300,0);
		rTexture.draw(*text);

		makeLabel(text, "Area", 27, 135, 350,0);
		rTexture.draw(*text);

		makeLabel(text, separate(to_string((unsigned long long)(c->population/c->density))), 27, 375, 350,0);
		rTexture.draw(*text);

		makeLabel(text, "All C-19 cases", 20, 40, 528,1);
		rTexture.draw(*text);

		makeLabel(text, "Total deaths", 20, 40, 578,1);
		rTexture.draw(*text);

		makeLabel(text, "Was never ill", 20, 40, 628,1);
		rTexture.draw(*text);

		makeLabel(text, (btn?"Hide graph":"Show graph"), 30, 396, 772, 0);
		rTexture.draw(*text);

		rTexture.display();
		result.setTexture(rTexture.getTexture());
		
		result.setPosition(Cx+(mapSize.x/2-525)*wheel,Cy-(300)*wheel);

		result.setScale(wheel, wheel);

		return result;
	};
};