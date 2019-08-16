#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <atomic>

namespace Summit::Framegraph
{
	class ResourceBase;
	class RenderTaskBuilder;

	class RenderTaskBase
	{
		friend class RenderTaskBuilder;

	public:
		explicit RenderTaskBase(std::string name);

		RenderTaskBase(const RenderTaskBase&) = delete;
		RenderTaskBase(RenderTaskBase&&) = delete;

		virtual ~RenderTaskBase() = default;

		RenderTaskBase& operator=(const RenderTaskBase&) = delete;
		RenderTaskBase& operator=(RenderTaskBase&&) = delete;

		uint32_t GetId() const { return mId; }
		const std::string& GetName() const { return mName; }
		void SetName(std::string name) { mName = std::move(name); }

	private:
		uint32_t mId{ 0 };
		std::string mName;

		std::vector<const ResourceBase*> mCreatedResources;
  		std::vector<const ResourceBase*> mReadResources;
  		std::vector<const ResourceBase*> mWriteResources;
	};

	RenderTaskBase::RenderTaskBase(std::string name)
		: mName(std::move(name))
	{
		static std::atomic<uint32_t> s_id{ 0 };
		mId = s_id.fetch_add(1, std::memory_order_relaxed);
	}
}
