#pragma once

#include <string>

namespace Summit::Framegraph
{
	class Framegraph;
	class RenderTaskBase;

	class RenderTaskBuilder
	{
	public:
		explicit RenderTaskBuilder(Framegraph* framegraph, RenderTaskBase* renderTask) : mFramegraph(framegraph), mRenderTask(renderTask){}

		template<typename R, typename D>
		R* CreateResource(std::string name, const D& description);

		template<typename R>
		R* ReadResource(R* resource);

		template<typename R>
		R* WriteResource(R* resource);

	protected:
		Framegraph* mFramegraph{ nullptr };
		RenderTaskBase* mRenderTask{ nullptr };
	};
}