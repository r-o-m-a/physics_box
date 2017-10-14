#include "Window.hpp"
#define YWINDOW 100
#define TIMEUPDATE 25
#define SCALE 100
#define TIMESTEP (TIMEUPDATE/1000)
#define ITERATIONS 10
#define POS_ITERATIONS 2


Window::Window():
	
	sfgui(),
	toolsWindow(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
	
	box(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 100)),
	playBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 80)),
	playButton(sfg::Button::Create("Play")),
	pauseButton(sfg::Button::Create("Pause")),
	clearButton(sfg::Button::Create("Clear")),
	
	toolBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5)),
	upperBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5)),	
	circleButton(sfg::Button::Create("Circle")),
	rectangleButton(sfg::Button::Create("Rectangle")),
	
	lowerBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5)),
	polygonButton(sfg::Button::Create("Polygon")),
	selectButton(sfg::Button::Create("Select")),
	
	objectWindow(sfg::Window::Create(sfg::Window::Style::BACKGROUND)),
    objectBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 30)),
	backBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20)),	
	firstBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20)),	
	secondBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 25)),	
	thirdBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20)),	
	fourthBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20)),	
	fifthBox(sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5)),	
	
	backButton(sfg::Button::Create("Back")),
	checkButton(sfg::CheckButton::Create("")),
	frictionEntry(sfg::Entry::Create()),
	densityEntry(sfg::Entry::Create()),
	restitutionEntry(sfg::Entry::Create()),
	
	staticLabel(sfg::Label::Create("Static")),
	frictionLabel(sfg::Label::Create("Friction")),
	densityLabel(sfg::Label::Create("Density")),
	restitutionLabel(sfg::Label::Create("Restitution")),
	
	deleteButton(sfg::Button::Create("Delete")),
	applyButton(sfg::Button::Create("Apply")),
	
	//Box2D
	selected(nullptr),
	world(b2Vec2::b2Vec2(0.0f, -10.0f)),
	clock(),
	paused(false)

{	
	groundBodyDef.position.Set(XSIZE / SCALE / 2, -((YSIZE - YWINDOW / 2)/SCALE));
	groundBody = world.CreateBody(&groundBodyDef);
	groundBox.SetAsBox(XSIZE / SCALE / 2, YWINDOW / SCALE / 2);
	groundBody->CreateFixture(&groundBox, 0.0f);
	
	renderWindow.create(sf::VideoMode(XSIZE, YSIZE), "Physics sandbox", sf::Style::Close);
    tbackground.loadFromFile("pics/back.jpg");
    background.setTexture(tbackground);
	
	desktop.Add(objectWindow);
	desktop.Add(toolsWindow);
	
	
	toolsWindow->Add(box);
	box->Pack(playBox);
	box->Pack(toolBox);
	
	playBox->Pack(playButton);
	playBox->Pack(pauseButton);
	playBox->Pack(clearButton);
	toolBox->Pack(upperBox);
	toolBox->Pack(lowerBox);
	
	upperBox->Pack(circleButton);
	upperBox->Pack(rectangleButton);
	lowerBox->Pack(polygonButton);
	lowerBox->Pack(selectButton);
	
	toolsWindow->SetAllocation(sf::FloatRect(0, YSIZE-YWINDOW, XSIZE, YWINDOW));
	
	backBox->Pack(backButton);
	firstBox->Pack(staticLabel);
	firstBox->Pack(frictionLabel);
	secondBox->Pack(checkButton);
	secondBox->Pack(frictionEntry);
	thirdBox->Pack(densityLabel);
	thirdBox->Pack(restitutionLabel);
	fourthBox->Pack(densityEntry);
	fourthBox->Pack(restitutionEntry);
	fifthBox->Pack(deleteButton);
	fifthBox->Pack(applyButton);
	
	objectBox->Pack(backBox);
	objectBox->Pack(firstBox);
	objectBox->Pack(secondBox);
	objectBox->Pack(thirdBox);
	objectBox->Pack(fourthBox);
	objectBox->Pack(fifthBox);
	
	objectWindow->Add(objectBox);
	objectWindow->SetAllocation(sf::FloatRect(0, YSIZE-YWINDOW, XSIZE, YWINDOW));
	
	connectSignals();
	
	buttonPressed = ButtonPressed::NO;
	
	std::srand(unsigned(std::time(0)));
	
    sfg::Context::Get().GetEngine().SetProperties(".windows"
                                            "{"
                                                "BackgroundColor: #DEB887ee;"
                                                "BorderColor: #DEB887ee;"
                                            "}");
    sfg::Context::Get().GetEngine().SetProperties(".smallButtons"
                                            "{"
                                                "BackgroundColor: #A0522Dee;"
                                                "BorderColor: #A0522Dee;"
                                                "BorderWidth: 2f;"
                                                "FontSize: 14;"
                                                "BorderColorShift: 50;"
                                            "}");
											
	frictionLabel->SetId("l");
	staticLabel->SetId("l");
	densityLabel->SetId("l");
	restitutionLabel->SetId("l");
	sfg::Context::Get().GetEngine().SetProperty<sf::Color>( "Label#l", "Color", sf::Color(139, 69, 19) );
	sfg::Context::Get().GetEngine().SetProperty( "Label#l", "FontSize", 15 );

	circleButton->SetClass("smallButtons");
	rectangleButton->SetClass("smallButtons");
	polygonButton->SetClass("smallButtons");
	selectButton->SetClass("smallButtons");
	playButton->SetClass("smallButtons");
	pauseButton->SetClass("smallButtons");
	clearButton->SetClass("smallButtons");
	deleteButton->SetClass("smallButtons");
	applyButton->SetClass("smallButtons");
	backButton->SetClass("smallButtons");
	checkButton->SetClass("smallButtons");
	toolsWindow->SetClass("windows");
	objectWindow->SetClass("windows");
	frictionEntry->SetClass("smallButtons");
	densityEntry->SetClass("smallButtons");
	restitutionEntry->SetClass("smallButtons");
	
	playButton->SetId("p1");
    pauseButton->SetId("p2");
	
	ppButton = PPButton::PLAY;
	sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", "#8B4513ee");
	playButton->SetState(sfg::Widget::State::INSENSITIVE);
	objectWindow->Show(false);
}

Window::~Window()
{
	if(!entityList.empty())	
		for(auto it = entityList.begin(); it != entityList.end(); it++)
			delete *it;
}

void Window::connectSignals()
{
	circleButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton, this, ButtonPressed::CIRCLE) );
    polygonButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton, this, ButtonPressed::POLYGON) );
    rectangleButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton, this, ButtonPressed::RECTANGLE) );
    selectButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton, this, ButtonPressed::SELECT) );
	playButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedPlay, this, PPButton::PLAY) );
    pauseButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedPlay, this, PPButton::PAUSE) );
	clearButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::deleteAll, this) );
	backButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton2, this, ButtonPressed2::BACK) );
	deleteButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton2, this, ButtonPressed2::DELETE) );
	applyButton->GetSignal( sfg::Widget::OnLeftClick ).Connect(
                std::bind( &Window::clickedButton2, this, ButtonPressed2::APPLY) );
	frictionEntry->GetSignal( sfg::Entry::OnTextChanged ).Connect(
                std::bind( &Window::frictionChanged, this) );
	densityEntry->GetSignal( sfg::Entry::OnTextChanged ).Connect(
                std::bind( &Window::densityChanged, this) );
	restitutionEntry->GetSignal( sfg::Entry::OnTextChanged ).Connect(
                std::bind( &Window::restitutionChanged, this) );
}

void Window::update()
{
	while (renderWindow.isOpen())
    {
        // Process events
        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
			desktop.HandleEvent(event);
            if (event.type == sf::Event::Closed)
                renderWindow.close();
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				if((event.mouseButton.button == sf::Mouse::Button::Left) && isMouseInsideArea())
					createEntity();
				else if(event.mouseButton.button == sf::Mouse::Button::Right && (buttonPressed == ButtonPressed::POLYGON) 
				         && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					auto polygon = dynamic_cast<Polygon*>(entityList.back());
					polygon->newPolygon();
				 }
			}  
			else if(event.type == sf::Event::MouseButtonReleased)
			{
				if(!entityList.empty())
				{
					if((event.mouseButton.button == sf::Mouse::Button::Left && buttonPressed == ButtonPressed::POLYGON &&
						(entityList.back()->ready == false)))
							entityList.pop_back();					
				
					else if((event.mouseButton.button == sf::Mouse::Button::Left))
						enablePhysics();
				
					if((event.mouseButton.button == sf::Mouse::Button::Right) )
						enablePolygonPhisics();
				}
			}
			
        }		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMouseInsideArea())
		{
			changeEntitySize();
		}
		
		if(clock.getElapsedTime().asMilliseconds() >= TIMEUPDATE)
			update2();
			
		desktop.Update( TIMEUPDATE );
        renderWindow.clear();
        renderWindow.draw(background);

		for(auto it = entityList.begin(); it != entityList.end(); it++)
		{
			renderWindow.draw((*it)->returnDrawable());
		}
		sfgui.Display(renderWindow);
        renderWindow.display();
		
    }

}

void Window::clickedButton(ButtonPressed button)
{
	buttonPressed = button;
}

void Window::clickedButton2(ButtonPressed2 button)
{
	switch(button)
	{
		case ButtonPressed2::APPLY:
			if(densityEntry->GetText().isEmpty())
				densityEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetDensity()))) ;	
			selected->body->GetFixtureList()->SetDensity(stof(densityEntry->GetText().toAnsiString()));
			if(frictionEntry->GetText().isEmpty())
				frictionEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetFriction()))) ;
			selected->body->GetFixtureList()->SetFriction(stof(frictionEntry->GetText().toAnsiString()));
			if(restitutionEntry->GetText().isEmpty())
				restitutionEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetRestitution())));
			selected->body->GetFixtureList()->SetRestitution(stof(restitutionEntry->GetText().toAnsiString()));
			
			if(checkButton->IsActive())
				selected->body->SetType(b2_staticBody);
			else
				selected->body->SetType(b2_dynamicBody);
			selected->body->ResetMassData();
			selected->unsetSelected();
			break;
		case ButtonPressed2::BACK:
			selected->unsetSelected();
			break;
		case ButtonPressed2::DELETE:
			world.DestroyBody(selected->body);
			for(auto it = entityList.begin(); it != entityList.end(); it++)
			{
				if ((*it)->body == selected->body)
				{
					delete *it;
					entityList.erase(it);
					break;
				}
			}
			break;
	}
	selected = nullptr;
	
	objectWindow->Show(false);
	toolsWindow->Show(true);
	
	if(playButton->GetState() == sfg::Widget::State::INSENSITIVE)
		paused = false;
	buttonPressed = ButtonPressed::SELECT;
}

void Window::clickedPlay(PPButton button)
{
	switch(button)
	{
		case PPButton::PLAY:
			sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", "#8B4513ee");
            sfg::Context::Get().GetEngine().SetProperty("Button#p2", "BackgroundColor", "#A0522Dee");
			playButton->SetState(sfg::Widget::State::INSENSITIVE);
			pauseButton->SetState(sfg::Widget::State::NORMAL);
			paused = false;
			break;
		case PPButton::PAUSE:
			sfg::Context::Get().GetEngine().SetProperty("Button#p2", "BackgroundColor", "#8B4513ee");
            sfg::Context::Get().GetEngine().SetProperty("Button#p1", "BackgroundColor", "#A0522Dee");
			pauseButton->SetState(sfg::Widget::State::INSENSITIVE);
			playButton->SetState(sfg::Widget::State::NORMAL);	
			paused = true;
			break;
	}
}

bool Window::isMouseInsideArea()
{
	return (sf::Mouse::getPosition(renderWindow).x < XSIZE) && 
		   (sf::Mouse::getPosition(renderWindow).y < (YSIZE - YWINDOW)) &&
		   (sf::Mouse::getPosition(renderWindow).x > 1) &&
		   (sf::Mouse::getPosition(renderWindow).y > 1);
}

void Window::createEntity()
{
	switch(buttonPressed)
	{
		case ButtonPressed::CIRCLE:
			entityList.push_back(new Circle(sf::Mouse::getPosition(renderWindow)));	
			entityList.back()->setColor();
			break;
		case ButtonPressed::RECTANGLE:
			entityList.push_back(new Rectangle(sf::Mouse::getPosition(renderWindow)));
			entityList.back()->setColor();
			break;
		case ButtonPressed::POLYGON:
			entityList.push_back(new Polygon(sf::Mouse::getPosition(renderWindow)));
			entityList.back()->setColor();
			break;
		case ButtonPressed::SELECT:
			for(auto it = entityList.begin(); it != entityList.end(); it++)
			{
				for (b2Fixture* f = (*it)->body->GetFixtureList(); f; f = f->GetNext())
				{
					if(f->TestPoint(b2Vec2(float(sf::Mouse::getPosition(renderWindow).x) / SCALE,
						- float(sf::Mouse::getPosition(renderWindow).y) / SCALE)))
					{
						selected = *it;
						selected->setSelected();
						
						frictionEntry->SetText(sf::String(std::to_string(f->GetFriction()))) ;
						densityEntry->SetText(sf::String(std::to_string(f->GetDensity())));
						restitutionEntry->SetText(sf::String(std::to_string(f->GetRestitution())));
						if((*it)->body->GetType() == b2_dynamicBody)
							checkButton->SetActive(false);
						else
							checkButton->SetActive(true);
						objectWindow->Show(true);
						toolsWindow->Show(false);
						paused = true;
						buttonPressed = ButtonPressed::NO;
					}
				}
			}
		case ButtonPressed::NO:
			break;
	}
}

void Window::changeEntitySize()
{
	if(entityList.empty())
		return;
	if(entityList.back()->ready == true)
		return;
	switch(buttonPressed)
	{
		case ButtonPressed::CIRCLE:
		case ButtonPressed::RECTANGLE:
		case ButtonPressed::POLYGON:
			entityList.back()->change(sf::Mouse::getPosition(renderWindow));
			break;
		case ButtonPressed::NO:
			break;
	}
}

void Window::enablePhysics()
{
	if(entityList.empty())
		return;
	if(entityList.back()->ready == false)
		entityList.back()->ready = true;
	else
		return;
	if(buttonPressed != ButtonPressed::NO || buttonPressed != ButtonPressed::POLYGON)
	{		
		if(buttonPressed == ButtonPressed::CIRCLE)
		{
			entityList.back()->ready = true;
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.allowSleep = false;
			auto circleShape = dynamic_cast<sf::CircleShape&>(entityList.back()->returnDrawable());
			
			sf::Vector2f pos = circleShape.getPosition();
			bodyDef.position.Set(pos.x / SCALE, -pos.y / SCALE);
			entityList.back()->body = world.CreateBody(&bodyDef);
			
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;

			b2CircleShape dynamicBox;
			dynamicBox.m_radius = (circleShape.getRadius() / SCALE);
			fixtureDef.shape = &dynamicBox;
			entityList.back()->body->CreateFixture(&fixtureDef);	
			
		}
		else if (buttonPressed == ButtonPressed::RECTANGLE)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.allowSleep = false;
			
			auto rectShape = dynamic_cast<sf::RectangleShape&>(entityList.back()->returnDrawable());
			if(rectShape.getSize().x < 1 || rectShape.getSize().y < 1)
			{
				entityList.pop_back();
				return;
			}
			
			sf::Vector2f pos = rectShape.getPosition();
			bodyDef.position.Set(pos.x / SCALE, -pos.y / SCALE);
			entityList.back()->body = world.CreateBody(&bodyDef);
			
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(rectShape.getSize().x / SCALE / 2, rectShape.getSize().y / SCALE / 2);
			fixtureDef.shape = &dynamicBox;
			entityList.back()->body->CreateFixture(&fixtureDef);	
		}
	}
}

void Window::enablePolygonPhisics()
{
	if(entityList.back()->ready == true)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;
		auto polyShape = dynamic_cast<sf::ConvexShape&>(entityList.back()->returnDrawable());
		if(polyShape.getPointCount() > 8)
		{
			entityList.pop_back();
			return;
		}
		sf::Vector2f pos = polyShape.getPosition();
		bodyDef.position.Set(pos.x / SCALE, -pos.y / SCALE);
		entityList.back()->body = world.CreateBody(&bodyDef);
		
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		
		b2PolygonShape dynamicBox;
		b2Vec2* array = new b2Vec2[polyShape.getPointCount()];
		for(int i = 0; i < polyShape.getPointCount(); i++)
		{
			array[i] = b2Vec2(polyShape.getPoint(i).x / SCALE , - polyShape.getPoint(i).y / SCALE );
		}
		dynamicBox.Set(array, polyShape.getPointCount());
		delete [] array;
		fixtureDef.shape = &dynamicBox;
		entityList.back()->body->CreateFixture(&fixtureDef);
	}
}
void Window::update2()
{
	if(!paused)
	{
		world.Step(clock.restart().asSeconds(), ITERATIONS, POS_ITERATIONS);
		
		for(auto it = entityList.begin(); it != entityList.end(); it++)
		{
			(*it)->update();
		}
	}
	else
		clock.restart();
}

void Window::frictionChanged()
{
	if(frictionEntry->GetText().isEmpty())
		return;
	try{
	float value = stof(frictionEntry->GetText().toAnsiString());
	if(value < 0.0f || value > 1.0f)
		frictionEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetFriction()))) ;
	}
	catch(...){
		frictionEntry->SetText(sf::String(""));
	}
	
}

void Window::restitutionChanged()
{
	if(restitutionEntry->GetText().isEmpty())
		return;
	try{
		float value = stof(restitutionEntry->GetText().toAnsiString());
		if(value < 0.0f || value > 1.0f)
			restitutionEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetRestitution())));
	}
	catch(...){
		restitutionEntry->SetText(sf::String(""));
	}
}

void Window::densityChanged()
{
	if(densityEntry->GetText().isEmpty())
		return;
	try{
		float value = stof(densityEntry->GetText().toAnsiString());
		if(value < 0.01f || value > 1000.0f)
			densityEntry->SetText(sf::String(std::to_string(selected->body->GetFixtureList()->GetDensity())));
	}
		catch(...){
		densityEntry->SetText(sf::String(""));
	}
}

void Window::deleteAll()
{
	if(!entityList.empty())
	{
		for(auto it = entityList.begin(); it != entityList.end(); it++)
		{
			world.DestroyBody((*it)->body);
			delete (*it);
		}
		entityList.clear();
	}
}