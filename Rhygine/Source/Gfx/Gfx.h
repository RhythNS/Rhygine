#pragma once

namespace Rhygine
{
	class Window;

	class Gfx
	{
	public:
		Gfx();
		virtual ~Gfx();

		virtual void OnWindowAdded(Window* m_window) = 0;
		virtual void OnWindowResized(Window* t_window) = 0;
		virtual void OnWindowRemoved(Window* m_window) = 0;

		virtual void Draw() = 0;

		static Gfx* GetInstance();

	private:
		static Gfx* s_instance;
	};
}
