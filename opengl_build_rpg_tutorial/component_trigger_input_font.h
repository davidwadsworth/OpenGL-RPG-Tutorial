#pragma once
#include "component_trigger_input.h"
#include <sstream>
#include "json.hpp"
#include "delimiter_split.h"
#include "component_material_color.h"


/*
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
					std::string image_src = fnt_json["pages"][0];

					auto texture_manager = gamestate->get_child("texture manager");
					auto texture_id = delimiter_split(delimiter_split(image_src.c_str(), '/').back().c_str(), '.')[0];


					if (!gamestate->get_child("texture manager")->has_component<Component::Texture>(texture_id))
						gamestate->get_child("texture manager")->add_id_component<Component::Texture>(texture_id)->load(image_src.c_str());


					auto& c_font_texture = *gamestate->get_child("texture manager")->get_component<Component::Texture>(texture_id);
					auto& c_font_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("font");

					auto& c_font_material = *entity_->add_component<Component::Material::Color>( c_font_texture, c_font_shader, "font", glm::vec3(0.0f, 0.0f, 0.0f));

					auto chars_node = font_node->first_node("chars");

					auto tex_width = static_cast<double>(tex_c->width);
					auto tex_height = static_cast<double>(tex_c->height);

					auto char_node = chars_node->first_node("char");

					auto glyph(new Entity());
					auto bm_glyph = glyph->add_component<Component::BitMapGlyph>();

					auto id = atoi(char_node->first_attribute("id")->value());

					auto x = atof(char_node->first_attribute("x")->value());
					auto y = atof(char_node->first_attribute("y")->value());
					auto w = atof(char_node->first_attribute("width")->value());
					auto h = atof(char_node->first_attribute("height")->value());

					auto src_vec4 = glm::vec4(x / tex_width, y / tex_height, (x + w) / tex_width, (y + h) / tex_height);

					auto src = glyph->add_component<Component::Src>(src_vec4);

					bm_glyph->x_off = atoi(char_node->first_attribute("xoffset")->value());
					bm_glyph->y_off = atoi(char_node->first_attribute("yoffset")->value());
					bm_glyph->advance = atoi(char_node->first_attribute("xadvance")->value());
					bm_glyph->width = static_cast<GLuint>(w);
					bm_glyph->height = static_cast<GLuint>(h);
					bm_glyph->glyph = id;

					font->add_child(glyph);

					bm_font->child_off = id - 1;
					for (; char_node; char_node = char_node->next_sibling())
					{
						auto glyph(new Entity());
						auto bm_glyph = glyph->add_component<Component::BitMapGlyph>();

						id = atoi(char_node->first_attribute("id")->value());

						x = atof(char_node->first_attribute("x")->value());
						y = atof(char_node->first_attribute("y")->value());
						w = atof(char_node->first_attribute("width")->value());
						h = atof(char_node->first_attribute("height")->value());

						src_vec4 = glm::vec4(x / tex_width, y / tex_height, (x + w) / tex_width, (y + h) / tex_height);

						src = glyph->add_component<Component::Src>(src_vec4);

						bm_glyph->x_off = atoi(char_node->first_attribute("xoffset")->value());
						bm_glyph->y_off = atoi(char_node->first_attribute("yoffset")->value());
						bm_glyph->advance = atoi(char_node->first_attribute("xadvance")->value());
						bm_glyph->width = static_cast<GLuint>(w);
						bm_glyph->height = static_cast<GLuint>(h);
						bm_glyph->glyph = id;

						font->add_child(glyph);
					}

					for (auto kerning_node = font_node->first_node("kerning"); kerning_node; kerning_node = kerning_node->next_sibling())
					{
						auto first = atoi(kerning_node->first_attribute("first")->value());
						auto second = atoi(kerning_node->first_attribute("second")->value());
						auto amount = atoi(kerning_node->first_attribute("amount")->value());

						auto bm_glyph = font[first].get_component<Component::BitMapGlyph>();

						bm_glyph->kerning.push_back({ second, amount });
					}
				}
			};
		}

	}
}