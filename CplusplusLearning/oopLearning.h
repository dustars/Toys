#pragma once

template <typename T>
class animal
{
public:
	animal() = default;
	virtual void move(); // dynamically bound, remember using reference or pointer of the objects
	virtual ~animal() = default;
protected:
};