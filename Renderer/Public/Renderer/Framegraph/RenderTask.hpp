#pragma once

#include "RenderTaskBase.hpp"

#include <cstdint>
#include <string>

namespace Summit::Framegraph
{
	class RenderTaskBuilder;
	class Framegraph;

	template<typename T>
	using SetupPhaseHandler = std::function<void(T&, RenderTaskBuilder&)>;

	template<typename T>
	using ExecutionPhaseHandler = std::function<void(const T&)>;


	template<typename T>
	class RenderTask : public RenderTaskBase
	{
		friend class Framegraph;

	public:
		explicit RenderTask(std::string name, const SetupPhaseHandler<T>& setupFunc, const ExecutionPhaseHandler<T>& execFunc);

		RenderTask(const RenderTask&) = delete;
		RenderTask(RenderTask&&) = delete;

		virtual ~RenderTask() = default;

		RenderTask& operator=(const RenderTask&) = delete;
		RenderTask& operator=(RenderTask&&) = delete;

		const T& GetData() const;

	private:
		void Setup(RenderTaskBuilder& builder);
		void Execute();

	private:
		T mData;
		const SetupPhaseHandler<T> mSetupFunc;
		const ExecutionPhaseHandler<T> mExecFunc;
	};

	template<typename T>
	RenderTask<T>::RenderTask(std::string name, const SetupPhaseHandler<T>& setupFunc, const ExecutionPhaseHandler<T>& execFunc)
		: RenderTaskBase(std::move(name))
		, mSetupFunc(setupFunc)
		, mExecFunc(execFunc)
	{}

	template<typename T>
	void RenderTask<T>::Setup(RenderTaskBuilder& builder)
	{
		mSetupFunc(mData, builder);
	}

	template<typename T>
	void RenderTask<T>::Execute()
	{
		mExecFunc(mData);
	}

	template<typename T>
	const T& RenderTask<T>::GetData() const
	{
		return mData;
	}
}
