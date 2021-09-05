#include <ECS/Coroutine/WaitForSeconds.h>
#include <Core/Time.h>
#include <Core/Window.h>

WaitForSeconds::WaitForSeconds(float seconds) : timer(seconds)
{
}

bool WaitForSeconds::IsDone()
{
    timer -= Window::GetInstance()->time.GetDelta();
    return timer < 0;
}
