#include <UI/Elements/UIFont.h>
#include <2D/SpriteBatch.h>

#include <vector>

void UIFont::InnerDraw(SpriteBatch* batch)
{
	RhyM::Vec2 singleSize = font->GetRegionSize() * textScale;

	// Split the string into a vector of strings. A new string begins when either a line break was found or
	// wordWrap is enabled and the width of a string goes over the width of the current bounds.
	std::vector<std::string> strings;
	{
		float currentXSize = 0;
		std::string currentLine;
		for (int i = 0; i < text.size(); i++)
		{
			// is new line character?
			if (text[i] == '\n')
			{
				strings.push_back(currentLine);
				currentXSize = 0;
				currentLine = "";
				continue; // dont add the character to the currentLine, simply discard it.
			}
			// is word wrap and over bounds?
			else if (wordWrap && currentXSize > bounds.width)
			{
				strings.push_back(currentLine);
				currentXSize = 0;
				currentLine = "";
			}

			// Add to the current size and add the character to the current line
			currentXSize += singleSize.x;
			currentLine += text[i];
		}

		strings.push_back(currentLine);
	}
	
	// iterate over each line
	for (int i = 0; i < strings.size(); i++)
	{
		// get the x and y based on the alignment specified.
		float y = 0;
		switch (vert)
		{
		case UIFont::VertAlignment::Up:
			y = bounds.y + bounds.height - (singleSize.y * (i + 1));
			break;

		case UIFont::VertAlignment::Middle:
		{
			int halfSize = static_cast<int>(strings.size()) / 2;
			if (strings.size() % 2 == 0) // even
			{
				y = (bounds.y + bounds.height / 2) + singleSize.y * (halfSize - i - 1);
			}
			else // uneven
			{
				y = (bounds.y + bounds.height / 2) + singleSize.y * ((float)(halfSize - i) - 0.5f);
			}
			break;
		}

		case UIFont::VertAlignment::Down:
			y = bounds.y + singleSize.y * i;
			break;

		default:
			assert(false);
		}

		// iterate over each character in the current line.
		for (int j = 0; j < strings[i].size(); j++)
		{
			float x = 0;
			switch (hori)
			{
			case UIFont::HoriAlignment::Left:
				x = bounds.x + singleSize.x * j;
				break;

			case UIFont::HoriAlignment::Middle:
			{
				int halfSize = static_cast<int>(strings[i].size()) / 2;
				if (strings[i].size() % 2 == 0) // even
				{
					x = (bounds.x + bounds.width / 2) - singleSize.x * (halfSize - j + 1);
				}
				else // uneven
				{
					x = (bounds.x + bounds.width / 2) - singleSize.x * ((float)(halfSize - j) + 0.5f);
				}
				break;
			}

			case UIFont::HoriAlignment::Right:
				x = bounds.x + bounds.width - (singleSize.x * (strings[i].size() - j));
				break;

			default:
				assert(false);
			}

			// sanity check to see if the font has a texture region for that character.
			assert(font->GetRegion(strings[i][j])->texture);

			// draw the current character to the gotten x and y coord.
			batch->Draw(font->GetRegion(strings[i][j]), x, y, GetGlobalZ(), singleSize.x, singleSize.y, 0, color);
		}
	}
}

void UIFont::Set(MonoFont* _font, bool _wordWrap, VertAlignment _vert, HoriAlignment _hori, float _textScale)
{
	font = _font;
	wordWrap = _wordWrap;
	vert = _vert;
	hori = _hori;
	textScale = _textScale;
}
