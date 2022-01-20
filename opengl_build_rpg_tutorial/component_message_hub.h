#pragma once
#include "component.h"
#include "component_trigger_service.h"
#include "delimiter_split.h"

namespace Component
{
	class MessageHub : public IComponent
	{
		Entity* services_;
	public:
		MessageHub(Entity* services)
			: services_(services)
		{}

		void message(std::string msg)
		{
			if (msg.empty())
				Logger::error("empty message", Logger::SEVERITY::HIGH);

			auto split = delimiter_split(msg.c_str(), ':');

			if (!(split.size() % 2))
				Logger::error("invalid message format. eg. 'add:player'", Logger::SEVERITY::HIGH);


			for (auto i = 0; i < split.size() / 2; i += 2)
			{
				auto service = services_->get_component<Component::Trigger::IService>(split[i]);
				service->message(split[i+1]);
			}

		}
	};
}