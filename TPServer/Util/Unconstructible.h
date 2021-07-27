#pragma once

class Unconstructible
{
public:
	Unconstructible(const Unconstructible&) = delete;
	Unconstructible& operator=(const Unconstructible&) = delete;
	Unconstructible(Unconstructible&&) = delete;
	Unconstructible& operator=(Unconstructible&&) = delete;

protected:
	Unconstructible() {}
	virtual ~Unconstructible() {}
};