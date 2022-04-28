#pragma once
#include "component_system.h"
#include <vector>
#include "component_rect.h"
#include "component_renderer.h"
/*
Draw x src and transforms from one material, where x = draw_calls_.

@author David Wadsworth
*/
namespace Component {
	namespace System {
		namespace Render
		{
			class Empty : public Component::ISystem
			{
			public:
				struct Block
				{
					Component::Transform* transform;
					Component::Src* src;
				};
			private:

				struct RenderGroup
				{
					Component::Material* material;
					std::vector<Block> blocks;
				};
				std::vector<RenderGroup> render_groups_;
				std::vector<Block> free_blocks_;
				std::vector<Block>::iterator curr_block_;

				std::unordered_map<std::string, std::vector<Block>> objects_;

				Component::Renderer& renderer_;
			public:

				Empty(std::vector<Block> free_blocks, Component::Renderer& renderer)
					: free_blocks_(free_blocks), renderer_(renderer)
				{
					curr_block_ = free_blocks_.begin();
				}

				std::vector<Block> get_blocks(std::size_t size)
				{
					std::vector<Block> r_blocks(curr_block_, curr_block_ + size);
					curr_block_ += size;
					return r_blocks;
				}

				void add_obj(std::string id, std::vector<Block> blocks) { objects_[id] = blocks; }
				std::vector<Block> get_obj(std::string id) { return objects_[id]; }

				void add_render_group(std::vector<Block> blocks, Component::Material* material)
				{
					for (auto& rg : render_groups_)	
						if (rg.material->id == material->id)
						{
							rg.blocks.insert(rg.blocks.end(), blocks.begin(), blocks.end());
							return;
						}

					render_groups_.push_back(RenderGroup{material, blocks});
				}

				void clear()
				{
					render_groups_.clear();
					curr_block_ = free_blocks_.begin();
				}

				void execute() override final
				{
					for (auto& rg : render_groups_)
						for (auto& block : rg.blocks)
							renderer_.draw(*block.src, *block.transform, *rg.material);
				}
			};
		}
	}
}

