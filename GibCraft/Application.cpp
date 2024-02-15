#include "Application.h"

Gibcraft* Gibcraft::instance = nullptr;

Gibcraft* Gibcraft::Instance()
{
	if (instance == nullptr)
		instance = new Gibcraft();

	return instance;
}

Gibcraft::Gibcraft()
{

}

Gibcraft::~Gibcraft()
{

}