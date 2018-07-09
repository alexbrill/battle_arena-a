#pragma once

bool startGame()
{
    RenderWindow window(VideoMode(800, 600), "ARENA BATTLE");
	menu(window);
	view.reset(FloatRect(0, 0, 800, 600));

    Font font;
    font.loadFromFile("fonts/8bitOperatorPlus8-Regular.ttf");
    Text text("", font, 20);
    text.setColor(Color::White);

	Image heroImage, enemyImage, fireBallImage, map_image, tipsImage;
	map_image.loadFromFile("img/items.png");
	heroImage.loadFromFile("img/hero.png");
	enemyImage.loadFromFile("img/god.png");
	fireBallImage.loadFromFile("img/fireball.png");
	//tipsImage.loadFromFile("img/tips.png");

	Texture map; map.loadFromImage(map_image);
	Sprite s_map; s_map.setTexture(map);
	int objectGenTimer = 0,
        numberOfPassiveEnemy = 3,
        tipsTimer = 0;
	float time;
	//bool isTips = false;
	Clock clock;
    Player p(heroImage, 250, 250, 60, 60); //создание экземпляра игрока
	std::list<Entity*> entities;

    //создание экземпляров врагов
    for (int i = 0; i < numberOfPassiveEnemy; ++i)
        entities.push_back(new PassiveEnemy(enemyImage, 300 + i * 120, 250 + i * 150, 100, 100));

	while (window.isOpen())
	{
		time = (clock.getElapsedTime().asMicroseconds()) / 800 ;
		clock.restart();
		Vector2i pixelPos = Mouse::getPosition(window);
        Vector2f pos = window.mapPixelToCoords(pixelPos);
		Event event;

		/*if (isTips)
		{
            tipsTimer += time;

            if (tipsTimer > 2000)
            { tipsTimer = 0; isTips = false; }
		}*/

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (Keyboard::isKeyPressed(Keyboard::R)) return true;//если R, то перезагружаем игру
            if (Keyboard::isKeyPressed(Keyboard::Escape)) return false;//если эскейп, то выходим из игры
            //if (Keyboard::isKeyPressed(Keyboard::Tab)) isTips = true;

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left)
                    entities.push_back(new FireBall(fireBallImage, p.getCoord().x + 20,
                                        p.getCoord().y + 20, 15, 15, pos.x, pos.y));
		}

		objectGenTimer += time;
		if (objectGenTimer > 3000) { randomMapGenerate(); objectGenTimer = 0; }

		if (p.life) p.update(time); //апдейт персонажа

		for (auto it = entities.begin(); it != entities.end(); it++)
            if ((*it)->life) (*it)->update(time); //апдейт пуль и врагов

		window.setView(view);
		window.clear(Color(0, 0, 0));

        for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}

        //if (tips) window.draw();
		window.draw(p.sprite); //отрисовка игрока

		for (auto it = entities.begin(); it != entities.end(); it++)
            if ((*it)->life) window.draw((*it)->sprite);//отрисовка посторонних объектов

        if (!p.life) return true;

		std::ostringstream playerHealthString; playerHealthString << p.health;
        text.setString("Health: " + playerHealthString.str() + "%");
        text.setPosition(view.getCenter().x - 350, view.getCenter().y - 300);
        window.draw(text);

		window.display();
	}

	for (auto it = entities.begin(); it != entities.end(); ++it)
        delete *it; //освобождение памяти
}

void gameRunning()
{
	if (startGame()) gameRunning();
}
