#ifndef LOGGER_H
#define LOGGER_H

#include "RS-OpenLog.h"

#include <unordered_map>
#include <source_location>
#include <iomanip>
#include <memory>

#include <iostream>

using namespace RSLog;


class Logger final {
public:
	static Logger& GetInstance() noexcept;
    ~Logger() 	{};

	// Retrieve Member Variables
	inline const bool				getShowLogLocation	() 	const noexcept { return m_logSettings.m_showLocation;  		}
	inline const uint8_t			getLogCodeTextWidth	()	const noexcept { return m_logSettings.m_widthOfCodeTextBox; }
	inline const uint8_t			getLogMsgMaxSize	()	const noexcept { return m_logSettings.m_logMsgMaxSize; 		}

	// Set Memmber Variables
	inline 	void SetShowLogLocation		(bool showLogLocation	 )	noexcept { m_logSettings.m_showLocation = showLogLocation; 	}
	inline 	void SetLogMsgMaxSize		(const uint8_t length	 )	noexcept { m_logSettings.m_logMsgMaxSize = length;		 	}
	inline 	void SetLogCodeTextWidth	(const uint8_t length	 )	noexcept { m_logSettings.m_widthOfCodeTextBox = length;	 	}
			bool SetLogFilter			(const std::string key)		noexcept;
			bool SetLogTarget			(const std::string target) 	noexcept;

	// Log Filter Interaction
	void	AddLogFilter	(std::unique_ptr<LogFilter> filter);
	bool	RemoveLogFilter	(const std::string& key);

	
	// Log Target Interaction
	void	AddLogTarget	(std::unique_ptr<LogTarget> target);
	bool	RemoveLogTarget	(const std::string& key);	

	bool 	Log	(const std::string& msg, const std::string& code, const std::source_location location=std::source_location::current());

private:
	Logger();
    const Logger &operator=(const Logger &rst) = delete;

	// Variables
	std::unordered_map<std::string, std::unique_ptr<LogFilter>> 	m_logFilters 	{ };
	std::unordered_map<std::string, std::unique_ptr<LogTarget>> 	m_logTargets	{ };

	// Settings for Logging
	LogSettings m_logSettings {true, true, true, true};

	// Current Profiles
	LogTarget*	m_logTarget	{ nullptr };
	LogFilter*	m_logFilter { nullptr };

};


// Non-Member Helper Functions
std::string 	FormatTime		(const std::chrono::time_point<std::chrono::system_clock>& time) 	noexcept; // Returns time in HH::MM::SS::XXX, XXX being milliseconds, it's in three digits
std::string     FormatLocation	(const std::source_location location, bool oneLine=true) 			noexcept;

#endif