#pragma once


class INavigator
{
protected:
	nlohmann::json json_;
public:
	INavigator(nlohmann::json json)
		: json_(json)
	{}

	virtual void init(Entity* e_gamestate) = 0;
	virtual int navigate() = 0;
};



