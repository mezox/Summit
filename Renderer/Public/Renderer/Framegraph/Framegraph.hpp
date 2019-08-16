#pragma once

#include "RenderTask.hpp"
#include "RenderTaskBuilder.hpp"
#include "Resource.hpp"

#include <vector>
#include <memory>

namespace Summit::Framegraph
{
	class ResourceBase;

	class Framegraph
	{
		friend class RenderTaskBuilder;

	public:
		Framegraph() = default;

		template<typename D>
		RenderTask<D>* AddRenderTask(std::string name, const SetupPhaseHandler<D>& setupFunc, const ExecutionPhaseHandler<D>& execFunc);

		template<typename D, typename I>
		Resource<D, I>* AddRetainedResource(std::string name, const D& description, I* impl);

	private:
		std::vector<std::unique_ptr<RenderTaskBase>> mRenderTasks;
  		std::vector<std::unique_ptr<ResourceBase>> mResources;
	};

	template<typename D>
	RenderTask<D>* Framegraph::AddRenderTask(std::string name, const SetupPhaseHandler<D>& setupFunc, const ExecutionPhaseHandler<D>& execFunc)
	{
		mRenderTasks.push_back(std::make_unique<RenderTask<D>>(std::move(name), setupFunc, execFunc));
		auto taskPtr = static_cast<RenderTask<D>*>(mRenderTasks.back().get());

		RenderTaskBuilder builder(this, taskPtr);
		taskPtr->Setup(builder);

		return taskPtr;
	}

	template<typename D, typename I>
	Resource<D, I>* Framegraph::AddRetainedResource(std::string name, const D& description, I* impl)
	{
		mResources.push_back(std::make_unique<Resource<D, I>>(std::move(name), description, impl));
		return static_cast<Resource<D, I>*>(mResources.back().get());
	}

	template<typename R, typename D>
	R* RenderTaskBuilder::CreateResource(std::string name, const D& description)
	{
		static_assert(std::is_same_v<typename R::DescriptorType, D>, "Description does not match the resource.");
		mFramegraph->mResources.push_back(std::make_unique<R>(std::move(name), mRenderTask, description));
		
		const auto resource = mFramegraph->mResources.back().get();
		mRenderTask->mCreatedResources.push_back(resource);
		return static_cast<R*>(resource);
	}

	template<typename R>
	R* RenderTaskBuilder::ReadResource(R* resource)
	{
		resource->mReaders.push_back(mRenderTask);
		mRenderTask->mReadResources.push_back(resource);
		return resource;
	}

	template<typename R>
	R* RenderTaskBuilder::WriteResource(R* resource)
	{
		resource->mWriters.push_back(mRenderTask);
		mRenderTask->mWriteResources.push_back(resource);
		return resource;
	}
}
