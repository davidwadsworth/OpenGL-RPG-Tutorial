#include "renderer.h"

Renderer::Renderer(std::vector<GLuint> attributes)
	: vbo_(0), vao_(0)
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

Renderer::Renderer(Renderer&& other)
	: vbo_(other.vbo_), vao_(other.vao_)
{
	other.vbo_ = 0;
	other.vao_ = 0;
}

Renderer& Renderer::operator=(Renderer&& other)
{
	//ALWAYS check for self-assignment
	if (this != &other)
	{
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

void Renderer::draw(Rect dest_rect, Rect src_rect)
{
	// first triangle

	// bot left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y + dest_rect.w);
	buffer_.push_back(src_rect.x);
	buffer_.push_back(src_rect.y + src_rect.w);

	// top right
	buffer_.push_back(dest_rect.x + dest_rect.z);
	buffer_.push_back(dest_rect.y);
	buffer_.push_back(src_rect.x + src_rect.z);
	buffer_.push_back(src_rect.y);

	// top left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y);
	buffer_.push_back(src_rect.x);
	buffer_.push_back(src_rect.y);

	// second triangle

	// bot left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y + dest_rect.w);
	buffer_.push_back(src_rect.x);
	buffer_.push_back(src_rect.y + src_rect.w);

	// bot right
	buffer_.push_back(dest_rect.x + dest_rect.z);
	buffer_.push_back(dest_rect.y + dest_rect.w);
	buffer_.push_back(src_rect.x + src_rect.z);
	buffer_.push_back(src_rect.y + src_rect.w);

	// top right
	buffer_.push_back(dest_rect.x + dest_rect.z);
	buffer_.push_back(dest_rect.y);
	buffer_.push_back(src_rect.x + src_rect.z);
	buffer_.push_back(src_rect.y);
}

void Renderer::flush()
{
	// rebuffer changes to data
	glBufferData(GL_ARRAY_BUFFER, this->buffer_.size() * sizeof(GLfloat), this->buffer_.data(), GL_STATIC_DRAW);

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, this->buffer_.size() / att_size_);

	// clear buffer for next cycle
	buffer_.clear();
}