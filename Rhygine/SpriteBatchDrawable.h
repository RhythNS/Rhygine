#pragma once

class SpriteBatch;
class SpriteBatchDrawable
{
public:
	virtual void Draw(SpriteBatch* batch) = 0;
};
