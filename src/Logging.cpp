#include "Logger.h"

Logger::Logger() {

}
Logger& Logger::GetInstance() noexcept{
	static Logger instance;

	return instance;
}

bool Logger::SetLogFilter	(const std::string  key)		noexcept{
	if( m_logFilters.contains(key) ){
		m_logFilter = m_logFilters.at(key).get();
		return true;
	}
	else
		return false;
}

bool Logger::SetLogTarget	(const std::string  key)		noexcept{
	if( m_logTargets.contains(key) ){
		m_logTarget = m_logTargets.at(key).get();
		return true;
	}
	else
		return false;
}

bool 			Logger::Log				(const std::string& msg, const std::string& code, const std::source_location location){

	if(!m_logFilter){
		std::cout << "error, no log filter present" << std::endl;
		return false;
	}
	else{

		// Verify code is present in the current LogFilter
		if(m_logFilter->IsAllowed(code)){
			LogData logBuffer { msg, code, location, std::chrono::system_clock::now()}; // Create the buffer to be referenced
		

			bool displayLog{ false };

			// If the log is set to all, automatically display the log wihtout adding anywhere.
			// If log is not all, then see if it is allowed
			if(m_logFilter->str() == "ALL") { displayLog = true; }
			else{ displayLog = m_logFilter->IsAllowed(logBuffer.m_code); }


			if(displayLog){
				if(m_logTarget){

					// TODO - Figure out how to call derived function from base class list
					return m_logTarget->Log(logBuffer, m_logSettings);
				}
				else
					throw std::invalid_argument{"No Log Target has been set."};
			}
			else
				return false;
		}
		else
			return false;		
	}
	
}

// Log Filter
void	Logger::AddLogFilter	(std::unique_ptr<LogFilter> filter){ 
	m_logFilters.try_emplace(filter->str(), std::move(filter));
}
bool	Logger::RemoveLogFilter	(const std::string& key){
	try{
		auto& foundFilter { m_logFilters.at(key) };		// Reference to the logCode if found
		foundFilter.release();							// Delete LogCode
		m_logFilters.erase(key);
		return true;									
	}
	// If key could not be found, return false
	catch(std::out_of_range& e){
		return false;
	}
}

	
// Log Target
void	Logger::AddLogTarget	(std::unique_ptr<LogTarget> target){ 
	m_logTargets.try_emplace(target->str(), std::move(target));
}
bool	Logger::RemoveLogTarget	(const std::string& key){
	try{
		auto& foundTarget { m_logTargets.at(key) };		// Reference to the logCode if found
		foundTarget.release();							// Delete LogCode
		m_logTargets.erase(key);
		return true;									
	}
	// If key could not be found, return false
	catch(std::out_of_range& e){
		return false;
	}
}








