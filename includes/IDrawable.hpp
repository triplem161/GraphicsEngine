#ifndef IDRAWABLE_HPP
# define IDRAWABLE_HPP

class Shader;

class IDrawable
{
public:
	virtual void draw(Shader & shader) = 0;

};

#endif
