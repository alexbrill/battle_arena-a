#pragma once

void menu(RenderWindow & window) {
    bool isMenu = true;
	int menuNum = 0;
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("img/start.png");
	menuTexture2.loadFromFile("img/about.png");
	menuTexture3.loadFromFile("img/exit.png");
	aboutTexture.loadFromFile("img/about_text.png");
	menuBackground.loadFromFile("img/menu.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	menu1.setPosition(229, 214);
	menu2.setPosition(229, 288);
	menu3.setPosition(229, 363);
	menuBg.setPosition(0, 0);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menuNum = 0;
		Vector2i pixelPos = Mouse::getPosition(window);

        Event event;

		while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left)
                    if (IntRect(229, 214, 342, 60).contains(pixelPos)) menuNum = 1;
                    else if (IntRect(229, 288, 342, 60).contains(pixelPos)) menuNum = 2;
                    else if (IntRect(229, 363, 342, 60).contains(pixelPos)) menuNum = 3;
        }

        if (menuNum == 1) isMenu = false;
        if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
        if (menuNum == 3)  { window.close(); isMenu = false; }

		window.clear(Color(0, 0, 0));

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
}
