#pragma once

#include <React/ReactBodyBase.h>

namespace RhyReact
{
	class CollisionBody : public ReactBodyBase
	{
	public:
		~CollisionBody();
		void Init() override;

	protected:
		virtual void OnEnabled() override;
		virtual void OnDisabled() override;
	};
}
