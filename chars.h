#pragma omce

using namespace sf;

//------------------ТИП КООРДИНАТА--------------

struct point
{
    float x, y;
    point(float x, float y) : x(x), y(y) { }
};

//-----------------Базовый класс-------------

class Entity
{
protected:
    float x, y;
	float dx, dy, speed, moveTimer;
	int w,h;
	enum { left = 0, right, up, down, stay } state;

public:
    int health;
    bool life;
	Sprite sprite;
	Texture texture;

	Entity(Image&, float, float, int, int);
    virtual void update(float time) = 0;
    virtual void checkCollisionWithMap() = 0;
    virtual point getCoord() = 0;
    virtual ~Entity() {};
};

Entity::Entity(Image &image, float X, float Y, int W, int H)
{
    x = X; y = Y; w = W; h = H; moveTimer = 0;
    speed = 0; health = 100; dx = 0; dy = 0;
    life = true;
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setOrigin(w / 2, h / 2);
}

//-------------------КЛАСС ГЛАВНОГО ИГРОКА--------------------

class Player :public Entity
{
private:
    int playerScore;

    void control(float);
    void checkCollisionWithMap();

public:
    Player(Image &image, float X, float Y,int W,int H) : Entity(image, X, Y, W, H)
    {
        playerScore = 0; state = stay;
        sprite.setTextureRect(IntRect(0, 650, w, h));
        sprite.setScale(1.2f, 1.2f);
    }
    void update(float);
    point getCoord() { return point(x, y); }
    ~Player() { }
};

void Player::update(float time)
{
   control(time);
   switch (state)
   {
       case right: dx = speed; dy = 0; break;
       case left:dx = -speed; dy = 0; break;
       case up: dx = 0; dy = -speed; break;
       case down: dx = 0; dy = speed; break;
       case stay: break;
   }
   x += dx*time; y += dy*time; speed = 0;
   checkCollisionWithMap();
   sprite.setPosition(x + w / 2, y + h / 2);
   life = health > 0;
   if (life) setPlayerCoordinateForView(x, y);
}

void Player::control(float time)
{
   if (Keyboard::isKeyPressed)
   {
       if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
       {
            moveTimer += 0.005 * time;
            if (moveTimer > 9) moveTimer -= 9;
            state = left; speed = 0.1;
            sprite.setTextureRect(IntRect(64 * int(moveTimer), 583, 60, 60));
       }
       if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
       {
            moveTimer += 0.005 * time;
            if (moveTimer > 9) moveTimer -= 9;
           state = right; speed = 0.1;
           sprite.setTextureRect(IntRect(64 * int(moveTimer), 710, 60, 60));
       }

       if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
       {
            moveTimer += 0.005 * time;
            if (moveTimer > 9) moveTimer -= 9;
           state = up; speed = 0.1;
           sprite.setTextureRect(IntRect(64 * int(moveTimer), 520, 60, 60));
       }

       if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
       {
            moveTimer += 0.005 * time;
            if (moveTimer > 9) moveTimer -= 9;
           state = down; speed = 0.1;
           sprite.setTextureRect(IntRect(64 * int(moveTimer), 650, 60, 60));
       }
   }
}

void Player::checkCollisionWithMap()
{
    for (int i = y / 32; i < (y + h) / 32; ++i)
        for (int j = x / 32; j < (x + w) / 32; ++j)
            if (TileMap[i][j] == '0')
            {
                if (dy > 0) y = i * 32 - h;
                if (dy < 0) y = i * 32 + 32;
                if (dx > 0) x = j * 32 - w;
                if (dx < 0) x = j * 32 + 32;
            }
            else if (TileMap[i][j] == 'f')
            {
                TileMap[i][j] = ' '; health -= 40;
                if (health < 0) health = 0;
            }
            else if (TileMap[i][j] == 'h')
            {
                TileMap[i][j] = ' '; health += 20;
                if (health > 100) health = 100;
            }
}

//-------------------КЛАСС ВРАГА--------------------

class PassiveEnemy :public Entity
{
private:
    int choice;

	void checkCollisionWithMap();

public:
	PassiveEnemy(Image &image, float X, float Y, int W, int H) : Entity(image, X, Y, W, H)
	{
        sprite.setTextureRect(IntRect(0, 0, w, h));
        dx = 0.1; dy = 0.1; choice = 0;
	}
	void update(float);
	point getCoord() { return point(x, y); }
	~PassiveEnemy() { std::cout << "X" << std::endl; }
};

void PassiveEnemy::update(float time)
{
    if (moveTimer > 500) moveTimer = 0;
    if (moveTimer == 0) choice = rand() % 5;
    moveTimer += time;

    switch(choice)
    {
        case up: y -= dy * time; break;
        case down: y += dy * time; break;
        case left: x -= dx * time; break;
        case right: x += dx * time; break;
        case stay: break;
    }

    checkCollisionWithMap();
    sprite.setPosition(x + w / 2, y + h / 2);
    life = health > 0;
}

void PassiveEnemy::checkCollisionWithMap()
{
    for (int i = y / 32; i < (y + h) / 32; ++i)
        for (int j = x / 32; j < (x + w) / 32; ++j)
            if (TileMap[i][j] == '0')
            {
                x = 40 + rand() % 1000;
                y = 40 + rand() % 800;
            }
}

//-------------------КЛАСС ФАЙЕРБОЛЛ--------------------

class FireBall : public Entity
{
private:
    float xx, yy, distance;

public:
    FireBall(Image &image, float X, float Y, int W, int H, float XX, float YY) : Entity(image, X, Y, W, H)
    {
        sprite.setTextureRect(IntRect(0, 0, w, h));
        sprite.setScale(1.5f, 1.5f);
        dx = 0.5; dy = 0.5;
        xx = XX; yy = YY; distance = 1;
    }
    void update(float);
    void checkCollisionWithMap();
    point getCoord() { return point(x, y); }
    ~FireBall() { std::cout << "0"; }
};

void FireBall::checkCollisionWithMap()
{
    for (int i = y / 32; i < (y + h) / 32; ++i)
        for (int j = x / 32; j < (x + w) / 32; ++j)
            if (TileMap[i][j] == 's' || TileMap[i][j] == 'f') TileMap[i][j] = ' ';
}

void FireBall::update(float time)
{
    distance = sqrt((xx - x)*(xx - x) + (yy - y)*(yy - y));
    checkCollisionWithMap();

    if (distance > 0.3)
    {
        moveTimer += 0.005 * time;
        if (moveTimer > 3) moveTimer -= 3;
        x += dx * time * (xx - x) / distance;
        y += dy * time * (yy - y) / distance;
        sprite.setTextureRect(IntRect(15 * int(moveTimer), 0, 15, 15));
        sprite.setPosition(x + w / 2, y + h / 2);
    }
    else
    {
        life = false; x = 0; y = 0;
    }
}
