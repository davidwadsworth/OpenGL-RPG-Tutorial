#pragma once
#include "component_trigger_input.h"
#include <sstream>
#include "json.hpp"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Font : public Component::Trigger::IInput
			{
				std::string path_;
			public:
				Font(std::string name, std::string font_path)
					: Component::Trigger::IInput(name), path_(font_path)
				{}
			private:
				void create(Entity* gamestate) override final
				{
					// load font from file
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
						Logger::error("Failed to read tilemap file! path = " + path_, Logger::MEDIUM);
						return;
					}

					// parse into json obj
					auto fnt_json = nlohmann::json::parse(fnt_stream);
					const auto font_node = font_doc.first_node("font");

					auto common_node = font_node->first_node("common");
					auto info_node = font_node->first_node("info");

					auto font = new Entity();

					auto bm_font = font->add_component<Component::BitMapFont>();

					bm_font->font = info_node->first_attribute("face")->value();
					bm_font->line_h = atoi(common_node->first_attribute("lineHeight")->value());

					auto page_node = font_node->first_node("pages")->first_node("page");

					auto texture_manager = Game::global_objects["texture manager"].get();
					auto image_source = page_node->first_attribute("file")->value();
					auto texture_id = HelpersDelimiter::split(image_source, '.')[0];

					Component::Texture* tex_c = nullptr;

					if (texture_manager->has_child(texture_id))
						tex_c = texture_manager->get_child(texture_id)->get_component<Component::Texture>();
					else
					{
						tex_c = texture_manager->add_child(new Entity(), texture_id)->add_component<Component::Texture>();
						auto file_path = std::string("resources/images/") + std::string(image_source);
						HelpersTexture::load(tex_c, file_path.c_str());
					}

					auto text_shader = Game::global_objects["shader manager"]->get_child("text")->get_component<Component::Shader>();

					auto c_material = font->add_component<Component::Material>(new Materials::Color(tex_unit_, glm::vec3(0.0f, 0.0f, 0.0f)), tex_c, text_shader, "font");

					auto chars_node = font_node->first_node("chars");

					auto references = map["references"].get();
					HelpersReferences::add_game_obj_subscriber(references, "font");

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

