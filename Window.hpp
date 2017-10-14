#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <list>
#include <iostream>

#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Polygon.hpp"

class Window
{
private:
	const int XSIZE = 1000;
	const int YSIZE = 700;
	
	enum ButtonPressed
	{
		CIRCLE,
		RECTANGLE,
		POLYGON,
		SELECT,
		NO
	} buttonPressed;
	
	enum PPButton
	{
		PLAY,
		PAUSE
	} ppButton;
	
	enum ButtonPressed2
	{
		BACK,
		DELETE,
		APPLY
	};
	
	std::list<Entity*> entityList;
	
	
	sf::RenderWindow renderWindow;
	sf::Texture tbackground;
	sf::Sprite background;
	
	sfg::SFGUI sfgui;
	sfg::Desktop desktop;
	
	//TOOLS WINDOW
	sfg::Window::Ptr toolsWindow;

	sfg::Box::Ptr box;
	sfg::Box::Ptr playBox;
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr pauseButton;
	sfg::Button::Ptr clearButton;
	
	sfg::Box::Ptr toolBox;
	sfg::Box::Ptr upperBox;
	sfg::Button::Ptr circleButton;
	sfg::Button::Ptr rectangleButton;
	
	sfg::Box::Ptr lowerBox;
	sfg::Button::Ptr polygonButton;
	sfg::Button::Ptr selectButton;
	
	//OBJECT WINDOW
	sfg::Window::Ptr objectWindow;
	sfg::Box::Ptr objectBox;
	sfg::Box::Ptr backBox;
	sfg::Box::Ptr firstBox;
	sfg::Box::Ptr secondBox;
	sfg::Box::Ptr thirdBox;
	sfg::Box::Ptr fourthBox;
	sfg::Box::Ptr fifthBox;
	
	sfg::Button::Ptr backButton;
	
	sfg::CheckButton::Ptr checkButton;
	sfg::Entry::Ptr frictionEntry;
	sfg::Entry::Ptr densityEntry;
	sfg::Entry::Ptr restitutionEntry;
	
	sfg::Label::Ptr staticLabel;
	sfg::Label::Ptr frictionLabel;
	sfg::Label::Ptr densityLabel;
	sfg::Label::Ptr restitutionLabel;
	
	sfg::Button::Ptr deleteButton;
	sfg::Button::Ptr applyButton;
	
	//BOX2D
	Entity* selected;
	b2World world;
	b2BodyDef groundBodyDef;
	b2Body* groundBody;
	b2PolygonShape groundBox;
	
	sf::Clock clock;
	sf::RectangleShape ground;
	
	bool paused;

	
	void connectSignals();
	bool isMouseInsideArea();
	void createEntity();
	void clickedButton(ButtonPressed);
	void clickedPlay(PPButton);
	void clickedButton2(ButtonPressed2);
	void enablePhysics();
	void enablePolygonPhisics();
	void changeEntitySize();
	
	void frictionChanged();
	void restitutionChanged();
	void densityChanged();
	
	void deleteAll();
	
public:
	Window();
	~Window();
	
	void update();
	void update2();
};

#endif // WINDOW_HPP
