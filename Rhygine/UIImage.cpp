#include "UIImage.h"
#include "SpriteBatch.h"

#include <cassert>

void UIImage::InnerDraw(SpriteBatch* batch)
{
	assert(image.texture);
	DrawTextureRegion(batch, &image);
}
