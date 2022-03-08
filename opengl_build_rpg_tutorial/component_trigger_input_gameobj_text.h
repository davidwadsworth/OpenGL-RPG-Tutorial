#pragma once
#include "component_trigger_input.h"
#include "component_vector.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_renderer.h"
#include "component_rect_bitmap_glyph.h"
#include "component_system_render_offset.h"

/*
Display text to the screen without regard to the map.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class Text : public Component::Trigger::IInput
				{
					float render_group_;
					std::string text_, font_;
					glm::vec2 pos_;
				public:
					Text(std::string name, float render_group, std::string font, std::string text, glm::vec2 pos)
						: Component::Trigger::IInput(name), font_(font), text_(text), pos_(pos)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						// get renderer
						auto& c_renderer = *gamestate->get_component<Component::Renderer>();

						// get relevant font info
						auto font = gamestate->get_child(font_);

						auto& c_font_material = *font->get_component<Component::Material>("material");

						auto line_h = font->get_component<Component::Integer>("line_h")->value;

						auto &c_position = *entity_->push_back_component<Component::Position>(pos_);

						auto x_pos = pos_.x;
						char prev_c = 0;

						auto e_glyphs = font->get_child("glyphs");

						std::vector<Component::Src*> src_vec;
						std::vector<Component::Transform*> transform_vec;

						for (auto c : text_)
						{
							if (c == ' ')
							{
								x_pos += line_h / 3.0f;
								continue;
							}

							auto c_bm_glyph = e_glyphs->get_component<Component::Rectangle::BitMapGlyph>(static_cast<std::size_t>(c));

							auto c_transform = entity_->push_back_component<Component::Transform>();

							c_transform->x = x_pos + c_bm_glyph->x_off + c_bm_glyph->check_kerning(prev_c);
							c_transform->y = pos_.y + c_bm_glyph->y_off;
							c_transform->w = c_bm_glyph->w;
							c_transform->h = c_bm_glyph->h;

							x_pos += c_bm_glyph->advance;

							src_vec.push_back(c_bm_glyph);
							transform_vec.push_back(c_transform);
						}

						auto csr_offset_render = entity_->add_id_component<Component::System::Render::Offset>("render", c_position, src_vec, transform_vec, c_font_material, c_renderer);
						gamestate->get_child("engine")->get_component<Component::Engine>("render")->add(csr_offset_render, render_group_);
					}
				};
			}
		}

	}
}

