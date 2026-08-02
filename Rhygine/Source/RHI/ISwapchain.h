#pragma once

#include "RHIDescriptors.h"

namespace Rhygine
{
	// Forward declarations
	class Texture;

	/**
	 * ISwapchain - Manages the backbuffers for a specific Rhygine::Window.
	 * Handles presentation and swapchain image acquisition.
	 */
	class ISwapchain
	{
	public:
		virtual ~ISwapchain() = default;

		// =============================================================================
		// Swapchain Management
		// =============================================================================

		/// <summary>
		/// Resizes the swapchain to the specified dimensions.
		/// </summary>
		/// <param name="t_width">New width in pixels.</param>
		/// <param name="t_height">New height in pixels.</param>
		virtual void Resize(uint32_t t_width, uint32_t t_height) = 0;

		/// <summary>
		/// Gets the current backbuffer texture.
		/// </summary>
		/// <returns>Pointer to the current backbuffer texture.</returns>
		[[nodiscard]] virtual Texture* GetBackbuffer() = 0;

		/// <summary>
		/// Gets the index of the current backbuffer.
		/// </summary>
		/// <returns>Index of the current backbuffer image.</returns>
		[[nodiscard]] virtual uint32_t GetCurrentBackbufferIndex() const = 0;

		/// <summary>
		/// Gets the number of images in the swapchain.
		/// </summary>
		/// <returns>Number of swapchain images.</returns>
		[[nodiscard]] virtual uint32_t GetImageCount() const = 0;

		/// <summary>
		/// Gets the width of the swapchain.
		/// </summary>
		/// <returns>Width in pixels.</returns>
		[[nodiscard]] virtual uint32_t GetWidth() const = 0;

		/// <summary>
		/// Gets the height of the swapchain.
		/// </summary>
		/// <returns>Height in pixels.</returns>
		[[nodiscard]] virtual uint32_t GetHeight() const = 0;

		/// <summary>
		/// Gets the format of the swapchain images.
		/// </summary>
		/// <returns>Format of the swapchain images.</returns>
		[[nodiscard]] virtual Format GetFormat() const = 0;

		/// <summary>
		/// Acquires the next image from the swapchain.
		/// Must be called before presenting.
		/// </summary>
		/// <returns>Index of the acquired image.</returns>
		virtual uint32_t AcquireNextImage() = 0;

		/// <summary>
		/// Presents the current backbuffer to the screen.
		/// </summary>
		virtual void Present() = 0;

		/// <summary>
		/// Waits for the GPU to finish the current frame's work before proceeding.
		/// Must be called at the start of each frame before recording new commands.
		/// </summary>
		virtual void WaitForCurrentFrame() = 0;

		/// <summary>
		/// Gets an opaque handle to the semaphore signaled when the next image is available.
		/// </summary>
		[[nodiscard]] virtual SyncSemaphore GetImageAvailableSemaphore() = 0;

		/// <summary>
		/// Gets an opaque handle to the semaphore that rendering must signal before presentation.
		/// </summary>
		[[nodiscard]] virtual SyncSemaphore GetRenderFinishedSemaphore() = 0;

		/// <summary>
		/// Gets an opaque handle to the fence for the current in-flight frame.
		/// </summary>
		[[nodiscard]] virtual SyncFence GetCurrentFence() = 0;

		/// <summary>
		/// Checks if the swapchain needs to be recreated (e.g., due to window resize).
		/// </summary>
		/// <returns>True if swapchain needs recreation.</returns>
		[[nodiscard]] virtual bool NeedsRecreation() const = 0;

		/// <summary>
		/// Marks the swapchain as recreated.
		/// </summary>
		virtual void MarkRecreated() = 0;
	};
}