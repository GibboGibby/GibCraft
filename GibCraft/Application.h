#pragma once

class Gibcraft
{
public:
	
	static Gibcraft* Instance();

	Gibcraft();
	~Gibcraft();

	//World* GetWorld();

private:
	static Gibcraft* instance;
};