#ifndef _SPN_PROFILER_HPP_
#define _SPN_PROFILER_HPP_

#include <chrono>
#include <vector>
#include <iostream>

namespace spn {
#define CODEPROFILES_RESERVATION_SIZE 100
	enum class TimeUnit {
		Nanoseconds,
		Microseconds,
		Milliseconds,
		Seconds
	};
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;
	using CallStack = std::vector<long>;

	struct CodeProfile {
		long tag;
		TimeUnit unit;
		long serial;
		double deltaTime;
		TimePoint startTime;

		CodeProfile(
			long aTag,
			TimeUnit aUnit,
			long aSerial,
			double aDeltaTime
			)
			:
			tag(aTag),
			unit(aUnit),
			serial(aSerial),
			deltaTime(aDeltaTime)
		{
			startTime = Clock::now();
		}
	};

	using ProfilerDb = std::vector<CodeProfile>;


	class Profiler{
	public:
		inline static Profiler& GetInstance() {
			static Profiler instance;
			return instance;
		}

		inline void SetSecondsAsTimeUnit() {
			preferredUnit = TimeUnit::Seconds;
		}

		inline void SetMillisAsTimeUnit() {
			preferredUnit = TimeUnit::Milliseconds;
		}

		inline void SetMicrosAsTimeUnit() {
			preferredUnit = TimeUnit::Microseconds;
		}

		inline void SetNanosAsTimeUnit() {
			preferredUnit = TimeUnit::Nanoseconds;
		}


		inline void ClearAndReset(){
			serialNumber = 0;
			db.clear();
			stk.clear();
		}

		inline void Begin(long tag){
			snippetTag = tag;
			db.emplace_back(
				CodeProfile(
				snippetTag,
				preferredUnit,
				serialNumber,
				0.0
				)
				);
			stk.push_back(serialNumber);
			++serialNumber;
		}

		inline void End(){
			long serial = stk.back();
			stk.pop_back();
			TimePoint startTime = db[serial].startTime;
			TimePoint endTime = Clock::now();
			auto duration =
				std::chrono::duration_cast<std::chrono::nanoseconds>
				(endTime - startTime);
			double unitDivisor = 1.0;
			switch (preferredUnit) {
			case TimeUnit::Microseconds:
				unitDivisor = 1e3;
				break;
			case TimeUnit::Milliseconds:
				unitDivisor = 1e6;
				break;
			case TimeUnit::Seconds:
				unitDivisor = 1e9;
				break;
			default:
				break;
			}
			db[serial].deltaTime = duration.count() / unitDivisor;
		}

		inline void Print() {
			for (const auto& codeProfile : db) {
				std::cout
					//<< codeProfile.serial << " "
					<< codeProfile.tag << " --- "
					<< codeProfile.deltaTime;
				switch (codeProfile.unit) {
				case TimeUnit::Nanoseconds:
					std::cout << " nanoseconds";
					break;
				case TimeUnit::Microseconds:
					std::cout << " microseconds";
					break;
				case TimeUnit::Milliseconds:
					std::cout << " milliseconds";
					break;
				case TimeUnit::Seconds:
					std::cout << " seconds";
					break;
				default:
					std::cout << " unknown unit";
					break;
				}
				std::cout << '\n';
			}
		}
	private:
		ProfilerDb db;
		CallStack stk;
		TimeUnit preferredUnit;
		long serialNumber;
		long snippetTag;
		Profiler() :
			preferredUnit(TimeUnit::Milliseconds),
			serialNumber(0),
			snippetTag(0L)
		{
			db.reserve(CODEPROFILES_RESERVATION_SIZE);
			stk.reserve(CODEPROFILES_RESERVATION_SIZE);
		}
		Profiler(const Profiler&) = delete;
		Profiler& operator=(const Profiler&) = delete;
	};
#undef CODEPROFILES_RESERVATION_SIZE

	class ProfilerScope
	{
	public:
		ProfilerScope(long tag){
			spn::Profiler::GetInstance().Begin(tag);
		}
		~ProfilerScope(){
			spn::Profiler::GetInstance().End();
		}
	};

	class ProfilerLimitedScope
	{
	public:
		bool profilerStarted;
		ProfilerLimitedScope(long num, long lowerLimit, long upperLimit){
			if (num >= lowerLimit && num <= upperLimit){
				spn::Profiler::GetInstance().Begin(num);
				profilerStarted = true;
			}
			else{
				profilerStarted = false;
			}
			
		}
		~ProfilerLimitedScope(){
			if (profilerStarted){
				spn::Profiler::GetInstance().End();
			}
			
		}
	};
}

#endif