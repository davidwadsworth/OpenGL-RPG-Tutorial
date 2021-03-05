#include "renderer.h"

constexpr auto VERTICES = 6u;

Renderer::Renderer(std::vector<GLuint> attributes, GLuint max_sprites)
	: vbo_(0), vao_(0), current_mat_(nullptr), max_sprites_(max_sprites)
{
	// calculate total attribute size for attrib pointer arithmatic
	att_size_ = 0;
	for (auto att : attributes)
		att_size_ += att;

	// generate buffers
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);

	// bind buffers
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// create and bind attributes to vbo 
	auto stride = 0ull;
	for (auto i = 0; i < attributes.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, att_size_ * sizeof(float), (GLvoid*)stride);
		stride += attributes[i] * sizeof(float);
	}
}

Renderer::Renderer(Renderer&& other) noexcept
	: vbo_(other.vbo_), vao_(other.vao_), current_mat_(nullptr), att_size_(other.att_size_), max_sprites_(other.max_sprites_)
{
	// make the assigning renderer useless
	other.vbo_ = 0;
	other.vao_ = 0;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept
{
	// check for self-assignment
	if (this != &other)
	{
		// destroy and replace
		release();
		vbo_ = other.vbo_;
		vao_ = other.vao_;
		other.vbo_ = 0;
		other.vao_ = 0;
	}

	return *this;
}

void Renderer::release()
{
	// delete buffers if they exist
	if (vbo_)
		glDeleteBuffers(1, &vbo_);
	if (vao_)
		glDeleteBuffers(1, &vao_);
}

void Renderer::begin()
{
	// make sure we aren't batching with previous material
	current_mat_ = nullptr;
}

void Renderer::draw(Rect src, Rect dest, Material& mat)
{
	// checks if buffer is over sprite limit or current material isn't set
	// finally checks if the current material has a different id from the new material
	if ((this->buffer_.size() >= static_cast<std::size_t>(this->max_sprites_) * this->max_sprites_ * VERTICES || !this->current_mat_) 
		|| this->current_mat_->id != mat.id)
	{
		// flush out current batch and start on the next one
		this->flush();
		this->current_mat_ = &mat;
	}

	// first triangle

	// bot left
	buffer_.push_back(dest.x);
	buffer_.push_back(dest.y + dest.h);
	buffer_.push_back(src.x);
	buffer_.push_back(src.y + src.h);

	// top right
	buffer_.push_back(dest.x + dest.w);
	buffer_.push_back(dest.y);
	buffer_.push_back(src.x + src.w);
	buffer_.push_back(src.y);

	// top left
	buffer_.push_back(dest.x);
	buffer_.push_back(dest.y);
	buffer_.push_back(src.x);
	buffer_.push_back(src.y);

	// second triangle

	// bot left
	buffer_.push_back(dest.x);
	buffer_.push_back(dest.y + dest.h);
	buffer_.push_back(src.x);
	buffer_.push_back(src.y + src.h);

	// bot right
	buffer_.push_back(dest.x + dest.w);
	buffer_.push_back(dest.y + dest.h);
	buffer_.push_back(src.x + src.w);
	buffer_.push_back(src.y + src.h);

	// top right
	buffer_.push_back(dest.x + dest.w);
	buffer_.push_back(dest.y);
	buffer_.push_back(src.x + src.w);
	buffer_.push_back(src.y);
}

void Renderer::draw(Component::Render render, Material& mat)
{
	Renderer::draw(render.src, render.dest, mat);
}

void Renderer::flush()
{
	if (this->buffer_.empty()) return;

	// double make sure the current batch is clear and ready to used
	if (!this->current_mat_)
	{
		this->buffer_.clear();
		return;
	}

	// set uniforms and use shader
	this->current_mat_->compile();

	// bind texture to appropriate slot
	this->current_mat_->bind();

	// rebuffer changes to data
	glBufferData(GL_ARRAY_BUFFER, this->buffer_.size() * sizeof(GLfloat), this->buffer_.data(), GL_STATIC_DRAW);

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(this->buffer_.size() / att_size_));

	// clear buffer for next cycle
	this->buffer_.clear();
}

void Renderer::end()
{
	// flush everything out
	this->flush();
}
