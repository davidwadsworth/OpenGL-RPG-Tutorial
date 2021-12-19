#pragma once
#include "component_trigger_input.h"
#include <sstream>
#include "json.hpp"
#include "delimiter_split.h"
#include "component_material_color.h"
#include "component_src_bitmap_glyph.h"
#include "component_template.h"

/*
Load in font bitmap fonts with json.
 
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Font : public Component::Trigger::IInput
			{
				std::string path_;
			public:
				Font(std::string name, std::string path)
					: Component::Trigger::IInput(name), path_(path)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);
					cti_observer.add_subscriber(name_);

					// load tilemap from file
					std::stringstream fnt_stream;

					try
					{
						// open files
						std::ifstream fnt_file(path_);

						// read into temp string streams
						fnt_stream << fnt_file.rdbuf();

						// close file streams
						fnt_file.close();
					}
					catch (std::exception e)
					{
						Logger::error("Failed to read font file! path = " + path_, Logger::MEDIUM);
						return;
					}

					// parse into json obj
					auto fnt_json = nlohmann::json::parse(fnt_stream);

					int line_h = fnt_json["common"]["lineHeight"];
					entity_->add_component<Component::Integer>(line_h);
					std::string image_src = fnt_json["pages"][0];

					auto texture_manager = gamestate->get_child("texture manager");
					auto texture_id = delimiter_split(delimiter_split(image_src.c_str(), '/').back().c_str(), '.')[0];

					if (!gamestate->get_child("texture manager")->has_component<Component::Texture>(texture_id))
						gamestate->get_child("texture manager")->add_id_component<Component::Texture>(texture_id)->load(image_src.c_str());

					auto& c_font_texture = *gamestate->get_child("texture manager")->get_component<Component::Texture>(texture_id);
					auto& c_font_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("font");

					auto& c_font_material = *entity_->add_component<Component::Color>( c_font_texture, c_font_shader, 3, glm::vec3(0.0f, 0.0f, 0.0f));

					auto e_glyphs = new Entity();
					entity_->add_id_child(e_glyphs, "glyphs");

					for (auto glyph_json : fnt_json["chars"])
					{
						std::size_t id = glyph_json["id"];

						auto& c_bm_glyph = *e_glyphs->add_id_component<Component::BitMapGlyph>(id);

						c_bm_glyph.x = glyph_json["x"];
						c_bm_glyph.y = glyph_json["y"];
						c_bm_glyph.w = glyph_json["width"];
						c_bm_glyph.h = glyph_json["height"];
						
						c_bm_glyph.x_off = glyph_json["xoffset"];
						c_bm_glyph.y_off = glyph_json["yoffset"];
						c_bm_glyph.advance = glyph_json["xadvance"];
						c_bm_glyph.glyph = static_cast<char>(id);
					}

					for (auto kerning_json : fnt_json["kerning"])
					{
						std::size_t first = kerning_json["first"];
						int second = kerning_json["second"];
						int amount = kerning_json["amount"];

						auto& c_bm_glyph = *e_glyphs->get_component<Component::BitMapGlyph>(first);

						c_bm_glyph.kerning.push_back({ second, amount });
					}
				}
			};
		}

	}
}