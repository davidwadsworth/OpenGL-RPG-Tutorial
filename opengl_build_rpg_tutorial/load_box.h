#pragma once
#include "load.h"
#include "component_rect.h"
#include "component_system_render_blockdraw.h"
#include "game.h"

namespace Load
{
	class Box : public ILoad
	{
	public:

		void load(nlohmann::json json, Entity* e_gamestate) override
		{
			float item_x = json["rect"]["x"];
			float item_y = json["rect"]["y"];
			float item_w = json["rect"]["w"];
			float offset_y = json["box"]["offset"];

			float scale = json["box"]["scale"];

			float box_h = json["box"]["height"];
			float box_w = json["box"]["width"];

			box_h *= scale;
			box_w *= scale;

			float corner_size = json["box"]["corner_size"];
			corner_size *= scale;

			std::string spritesheet_name = json["box"]["texture"];

			auto texture_ids = json["box"]["textures"].get<std::vector<std::string>>();

			auto offset_x = item_w / 2.0f - corner_size * 2.0f;
			auto box_x = item_x + offset_x;
			auto box_y = item_y + offset_y - box_h;
			auto is_speech_arrow = json["message"]["speech_box"] == "true";
			std::string textbox_name = json["textbox"];

			auto e_textarea = e_gamestate->get_child(textbox_name);
			auto& csr_empty = *e_textarea->get_component<Component::System::Render::BlockDraw>("block_draw");

			auto& c_renderer = *e_gamestate->get_component<Component::Renderer>("renderer");

			auto blocks = csr_empty.get_blocks(10);

			// box corners transforms
			blocks[0].transform->set(Rect(box_x, box_y, corner_size, corner_size));
			blocks[1].transform->set(Rect(box_x + box_w - corner_size, box_y, corner_size, corner_size));
			blocks[2].transform->set(Rect(box_x, box_y + box_h - corner_size, corner_size, corner_size));
			blocks[3].transform->set(Rect(box_x + box_w - corner_size, box_y + box_h - corner_size, corner_size, corner_size));

			// box side transforms
			blocks[4].transform->set(Rect(box_x + corner_size, box_y, box_w - corner_size * 2, corner_size));
			blocks[5].transform->set(Rect(box_x, box_y + corner_size, corner_size, box_h - corner_size * 2.0f));
			blocks[6].transform->set(Rect(box_x + box_w - corner_size, box_y + corner_size, corner_size, box_h - corner_size * 2.0f));
			blocks[7].transform->set(Rect(box_x + corner_size, box_y + box_h - corner_size, box_w - corner_size * 2.0f, corner_size));

			// center transform
			blocks[8].transform->set(Rect(box_x + corner_size, box_y + corner_size, box_w - corner_size * 2.0f, box_h - corner_size * 2.0f));

			// speech arrow

			if (is_speech_arrow)
				blocks[9].transform->set(Rect(box_x + corner_size, box_y + box_h - corner_size, corner_size * 2.0f, corner_size));
			else
				blocks[9].transform->set(Game::removed.x, Game::removed.y, 0.0f, 0.0f);

			// get textbox shader and texture
			auto e_spritesheet = e_gamestate->get_child(spritesheet_name);
			auto& c_ss_material = *e_spritesheet->get_component<Component::Material>("material");


			// get src rects
			std::vector<Component::Src*> box_srcs;
			auto i = 0;
			for (std::string texture_id : texture_ids)
				blocks[i++].src->set(*e_spritesheet->get_component<Component::Src>(texture_id));

		}
	};
}