#pragma once
#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_renderer.h"
#include "component_src_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_observer.h"

/*
Display text to the screen without regard to the map.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class Text : public Component::Trigger::IInput
				{
					std::string text_;
					glm::vec2 pos_;
				public:
					Text(std::string name, std::string text, glm::vec2 pos)
						: Component::Trigger::IInput(name), text_(text), pos_(pos)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("display text");

						// get renderer
						auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

						// get render systems
						auto& render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");

						auto font = gamestate->get_child("gilsans");

						// hacky way to get font material, might change this, might not.
						auto& c_font_material = *font->get_component<Component::Color>();

						auto line_h = font->get_component<Component::Integer>()->value;

						auto x_pos = pos_.x;
						char prev_c = 0;

						auto e_glyphs = font->get_child("glyphs");

						for (auto c : text_)
						{
							if (c == ' ')
							{
								x_pos += line_h / 3.0f;
								continue;
							}

							auto& c_bm_glyph = *e_glyphs->get_component<Component::BitMapGlyph>(static_cast<std::size_t>(c));

							auto& c_transform = *entity_->push_back_component<Component::Transform>();

							c_transform.x = x_pos + c_bm_glyph.x_off + c_bm_glyph.check_kerning(prev_c);
							c_transform.y = pos_.y + c_bm_glyph.y_off;
							c_transform.w = c_bm_glyph.w;
							c_transform.h = c_bm_glyph.h;

							x_pos += c_bm_glyph.advance;

							auto crs_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_bm_glyph, c_transform, c_font_material);
							render_systems.push_back(crs_draw);
						}
					}
				};
			}
		}

	}
}

