#pragma once
#include "Bindable.h"
#include "UnBindable.h"

/// <summary>
/// A Un- Bindable for changing the blend state of the pipeline.
/// </summary>
class BlendStateBindable : public Bindable, public UnBindable
{
public:
	BlendStateBindable();
	BlendStateBindable(D3D11_BLEND_DESC desc);

	void Bind();
	void UnBind();
private:
	/// <summary>
	/// Gets the default description for a blend enabled blend state.s
	/// </summary>
	D3D11_BLEND_DESC GetDefaultDescription();

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
};
