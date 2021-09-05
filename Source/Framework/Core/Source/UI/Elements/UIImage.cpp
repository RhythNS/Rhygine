#include <UI/Elements/UIImage.h>
#include <2D/SpriteBatch.h>

#include <cassert>

void UIImage::InnerDraw(SpriteBatch* batch)
{
	assert(image.texture);
	DrawTextureRegion(batch, &image);
}
