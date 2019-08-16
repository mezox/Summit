#pragma once

#include "ResourceBase.hpp"

#include <variant>
#include <memory>

namespace Summit::Framegraph
{
	template<typename DescT, typename ImplT>
	class Resource : public ResourceBase
	{
	public:
		using DescriptorType = DescT;
		using ResourceType = ImplT;

	public:
		Resource(std::string name, const RenderTaskBase* creator, const DescriptorType& desc);
		Resource(std::string name, const DescriptorType& description, ResourceType* actual);

		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;

		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;

		const DescriptorType& GetDescriptor() const { return mDescriptor; }
		ResourceType* GetResource() const;

	private:
		DescriptorType mDescriptor;
		std::variant<std::unique_ptr<ImplT>, ImplT*> mResource;
	};

	template<typename D, typename I>
	Resource<D, I>::Resource(std::string name, const RenderTaskBase* creator, const DescriptorType& desc)
		: ResourceBase(std::move(name), creator)
		, mDescriptor(desc)
		, mResource(std::make_unique<I>())
	{}

	template<typename D, typename I>
	Resource<D, I>::Resource(std::string name, const DescriptorType& description, ResourceType* resource)
		: ResourceBase(std::move(name), nullptr)
		, mDescriptor(description)
		, mResource(resource)
	{
		if(!resource)
		{
			//construct;
		}
	}

	template<typename D, typename I>
	typename Resource<D, I>::ResourceType* Resource<D, I>::GetResource() const
	{
		if(std::holds_alternative<std::unique_ptr<I>>(mResource))
			return std::get<std::unique_ptr<I>>(mResource).get();
		else
			return std::get<I*>(mResource);
	}
}
