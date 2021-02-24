#include "WaitForSeconds.h"
#include "Time.h"
#include "Window.h"

WaitForSeconds::WaitForSeconds(float seconds) : timer(seconds)
{
}

bool WaitForSeconds::IsDone()
{
    timer -= Window::GetInstance()->time.GetDelta();
    return timer < 0;
}
