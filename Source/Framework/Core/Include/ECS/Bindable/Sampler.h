#pragma once
#include <ECS/Bindable/Bindable.h>

/// <summary>
/// Representation of a sampler.
/// </summary>
class Sampler : public Bindable
{
public:
	/// <summary>
	/// Creates a sampler with the default description.
	/// </summary>
	/// <param name="slot">The slot to where it will be bound to.</param>
	Sampler(int slot);

	/// <summary>
	/// Creates a sampler with a custom description.
	/// </summary>
	/// <param name="slot">The slot to where it will be bound to.</param>
	/// <param name="desc">The custom desciption.</param>
	Sampler(int slot, D3D11_SAMPLER_DESC desc);

	void Bind();

	/// <summary>
	/// Gets a pointer to the sampler state.
	/// </summary>
	ID3D11SamplerState* Get();
	
	int slot;

private:
	/// <summary>
	/// Gets the default description
	/// </summary>
	D3D11_SAMPLER_DESC GetDefaultDescription();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerPointer;
};

