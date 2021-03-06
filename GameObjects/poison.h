class Poison : public Edible
{
private:
	uint8_t hit = 0;
public:
	Poison();
	~Poison();
	bool update(int16_t, int16_t); // Status of food
	uint8_t getHit() const;
};

Poison::Poison()
{
	foodColor = GREEN;
}

Poison::~Poison() {}

bool Poison::update(int16_t lizardHeadx, int16_t lizardHeady)
{
	if (foodPos.x == lizardHeadx && foodPos.y == lizardHeady)
	{
		hit++;
		return true;
	}
	else
		return false;
}
uint8_t Poison::getHit() const
{
	return hit;
}