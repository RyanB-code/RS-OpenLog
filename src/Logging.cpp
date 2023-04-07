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

bool Logger::AddActiveLogTarget	(const std::string  key)		{
	if( m_logTargets.contains(key) ){

		if(m_logTargetsActive.size() >= m_maxLogTargets){
			return false;
		}
		else{
			m_logTargetsActive.push_back(m_logTargets.at(key).get());
			return true;
		}
	}
	else
		throw std::invalid_argument("RS OpenLog: No known Log Targets could be found with key " + key);
}
bool Logger::RemoveActiveLogTarget	(const std::string key){
	int i{0};
	bool success{false};
	for(LogTarget* currentLogTarget : m_logTargetsActive){
		if(currentLogTarget){
			if(currentLogTarget->str() == key){
				m_logTargetsActive[i] = nullptr;
				success = true;
			}
		}
		++i;
	}

	return success;
}

bool 			Logger::Log				(const std::string& msg, const std::string& code, const std::source_location location){
	bool success {false};

	if(!m_logFilter){
		throw std::invalid_argument(ThrowMSG("There is no Log Filter applied"));
		return false;
	}

	for(LogTarget* currentLogTarget : m_logTargetsActive){
		if(currentLogTarget){
			// Verify code is present in the current LogFilter
			if(m_logFilter->IsAllowed(code)){
				LogData logBuffer { msg, code, location, std::chrono::system_clock::now()}; // Create the buffer to be referenced
			
				success = currentLogTarget->Log(logBuffer, m_logSettings); // TODO counting of how many Log calls returned false and able to report back if needed
			}
		}
	}
	return success;
}

// Log Filter
void		Logger::AddLogFilter	(std::unique_ptr<LogFilter> filter){ 
	m_logFilters.try_emplace(filter->str(), std::move(filter));
}
bool		Logger::RemoveLogFilter	(const std::string& key){
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
LogFilter* 	Logger::GetLogFilter 	(const std::string key){
	return FindInMap(key, m_logFilters);
}
	
// Log Target
void		Logger::AddLogTarget	(std::unique_ptr<LogTarget> target){ 
	m_logTargets.try_emplace(target->str(), std::move(target));
}
bool		Logger::RemoveLogTarget	(const std::string& key){
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
LogTarget* 	Logger::GetLogTarget 	(const std::string key){
	return FindInMap(key, m_logTargets);
}







