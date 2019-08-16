#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <atomic>

namespace Summit::Framegraph
{
	class RenderTaskBase;

	class ResourceBase
	{
	public:
		ResourceBase(std::string name, const RenderTaskBase* creator);
		ResourceBase(const ResourceBase&) = delete;
		ResourceBase(ResourceBase&&) = delete;

		virtual ~ResourceBase() = default;

		ResourceBase& operator=(const ResourceBase&) = delete;
		ResourceBase& operator=(ResourceBase&&) = delete;

		uint32_t GetId() const { return mId; }
		const std::string& GetName() const { return mName; }
		void SetName(std::string name) { mName = std::move(name); }

	private:
		uint32_t mId{ 0 };
		std::string mName{ "Undefined" };
		const RenderTaskBase* mCreator{ nullptr };

		std::vector<const RenderTaskBase*> mReaders;
		std::vector<const RenderTaskBase*> mWriters;
	};

	ResourceBase::ResourceBase(std::string name, const RenderTaskBase* creator)
		: mName(std::move(name)), mCreator(creator)
	{
		static std::atomic<uint32_t> s_id{ 0 };
		mId = s_id.fetch_add(1, std::memory_order_relaxed);
	}
}
