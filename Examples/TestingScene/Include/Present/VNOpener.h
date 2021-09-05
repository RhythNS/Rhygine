#pragma once
#include <UI/Elements/UIButton.h>
#include <UI/Elements/UIFont.h>
#include <ECS/Updatable.h>

/// <summary>
/// Displays a flashing notification that, when clicked, creates a new
/// PresentVNManager component, which creates the visual novel style textbox.
/// </summary>
class VNOpener : public UIButton, public Updatable
{
public:
	void Init();
	void OnClick();
	void Update();
private:
	UIFont* font = nullptr;

	TextureRegion reg;
	bool deleteReq = false;

	// ------ variables for flashing color ------
	unsigned char r = 0, g = 255, b = 255;
	bool rIncrementing = true;
	float timer = 0;
	float timeForFullR = 0.3f;
	// ------------------------------------------
};
