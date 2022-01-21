#pragma once
#include "component_trigger_service.h"
#include "delimiter_split.h"
#include "component_system_item_textbox.h"

namespace Component {
	namespace Trigger {
		namespace Service
		{
			class CreateTextBox : public Component::Trigger::IService
			{
				std::vector<std::string> msgs_;
				std::size_t msgs_iter_;
				Component::System::Item::TextBox& csi_textbox_;

				void message(std::string msg) override
				{
					msgs_ = delimiter_split(msg.c_str(), '|');
					msgs_iter_ = 0;
				}

			public:
				CreateTextBox(Component::System::Item::TextBox& csi_textbox)
					: csi_textbox_(csi_textbox), msgs_iter_(0)
				{}

				void execute(Entity* gamestate) override
				{

				}
			};
		}
	}

}