#pragma once
#include "load.h"
#include "component_system_render_blockdraw.h"

namespace Load
{
	class OptionBox : public ILoad
	{

		Rect rect_;
		std::string font_name_, align_h_, align_v_, textbox_name_, nav_;
		float font_sc_, render_group_;
		nlohmann::json cursor_json_;
		float padding_;
	public:

		void load(nlohmann::json json)
		{
			align_h_ = json["optionbox"]["alignmenth"];
			align_v_ = json["optionbox"]["alignmentv"];
			nav_ = json["optionbox"]["nav"];
			cursor_json_ = json["optionbox"]["cursor"];
			textbox_name_ = json["textbox_name"];
			font_name_ = json["textarea"]["font"];
			font_sc_ = json["textarea"]["font_scale"];

		}

		void execute(Entity* gamestate) override
		{
			auto& csr_block_draw = *gamestate->get_child(textbox_name_)->get_component<Component::System::Render::BlockDraw>("render");
			// load font draw options in alignment using two sets of blocks. 


		}
	};
}