#pragma once

#include <cstdint>
#include <string>

namespace Rhygine
{
	class Window
	{
	public:
		using WindowId = uint32_t;

		enum class FullscreenMode
		{
			Windowed, Borderless, Exclusive
		};

		Window() = delete;
		Window(WindowId t_id, bool t_primary, uint32_t t_width, uint32_t t_height, uint32_t t_pos_x, uint32_t t_pos_y);
		virtual ~Window() = default;

		[[nodiscard]] virtual uint32_t GetX() const;
		[[nodiscard]] virtual uint32_t GetY() const;

		[[nodiscard]] virtual uint32_t GetWidth() const;
		[[nodiscard]] virtual uint32_t GetHeight() const;

		[[nodiscard]] virtual bool IsPrimary() const;

		virtual void Resize(uint32_t t_pos_x, uint32_t t_pos_y, uint32_t t_width, uint32_t t_height) = 0;

		virtual bool SetTitle(const std::string& m_title) = 0;
		[[nodiscard]] virtual const std::string& GetTitle() const;

		virtual bool SetCaptureMouse(bool m_enable) = 0;
		[[nodiscard]] virtual bool IsCapturingMouse() const;

		virtual bool SetFullscreenMode(FullscreenMode m_mode) = 0;
		[[nodiscard]] virtual FullscreenMode GetFullscreenMode() const;

		[[nodiscard]] WindowId GetID() const;

		[[nodiscard]] virtual void* GetNativeHandle() const = 0;

	protected:
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_pos_x = 0;
		uint32_t m_pos_y = 0;
		FullscreenMode m_mode = FullscreenMode::Windowed;
		bool m_capture_mouse = false;
		std::string m_title = "Rhygine";

	private:
		WindowId m_id;
		bool m_primary = false;
	};
}
