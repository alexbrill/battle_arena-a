#pragma once

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                             f    f   0",
	"0   s      s                    f      0",
	"0                    h             f   0",
	"0                                      0",
	"0      f                               0",
	"0                 s                    0",
	"0                                      0",
	"0         s                            0",
	"0                      h       s       0",
	"0             f                        0",
	"0                       s              0",
	"0      s                               0",
	"0                                      0",
	"0             h                        0",
	"0                                s     0",
	"0      f         s              s      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0         s                            0",
	"0                           s          0",
	"0                     h              s 0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};


void randomMapGenerate()
{
    srand(time(0));
	int randomElementX = 0,
        randomElementY = 0,
        countStone = 2;

	while (countStone > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			if (rand() % 2) TileMap[randomElementY][randomElementX] = 's';
			else TileMap[randomElementY][randomElementX] = 'f';
			countStone--;
		}
	}
}




