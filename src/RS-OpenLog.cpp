#include "RS-OpenLog.h"

#include "Logger.h"

namespace RSLog{

	// Log Code
	LogCode::LogCode		(const std::string code) : m_code{code} 	{   }
	LogCode::~LogCode()													{	}
	const std::string LogCode::str() const noexcept						{ return m_code; }
	std::ostream& operator<<(std::ostream& os, const LogCode& lc){
		os << lc.str();
		return os;
	}



	// Log Fillter
	LogFilter::LogFilter    	(const std::string name) : m_name {name} 	{	}
	LogFilter::~LogFilter		()											{	}
	
	void 	LogFilter::AddLogCode		(LogCode* lc ){
		m_logCodesAllowed.try_emplace(lc->str(), std::make_unique<LogCode>(*lc));
		return;
	}
	void 	LogFilter::AddLogCode		(std::unique_ptr<LogCode> lc ){
		m_logCodesAllowed.try_emplace(lc->str(), std::move(lc));
		return;
	}

	bool 	LogFilter::RemoveLogCode  	(const std::string key){
		try{
			auto& foundLogCode { m_logCodesAllowed.at(key) };	// Reference to the logCode if found
			foundLogCode.release();								// Delete LogCode
			m_logCodesAllowed.erase(key);
			return true;									
		}
		// If key could not be found, return false
		catch(std::out_of_range& e){
			return false;
		}
	}
	bool	LogFilter::IsAllowed  		( const std::string logCodeKey  ) const {
		return m_logCodesAllowed.contains(logCodeKey);
	}
	const std::string 	LogFilter::str	() const noexcept {
		return m_name; 
	} 
	std::ostream& operator<<(std::ostream& os, const LogFilter& lf){
		os << lf.str();
		return os;
	}



	// Log Target
	LogTarget::LogTarget(const std::string name) : m_name{name} {
		
	}
	LogTarget::~LogTarget(){
		
	}
	bool LogTarget::Log (const LogData& log,  const LogSettings& settings){
		std::cout <<  "base\n";
		return false;
	}
	const std::string 	LogTarget::str	() const noexcept {
		return m_name; 
	} 
	std::ostream& operator<<(std::ostream& os, const LogTarget& lt){
		os << lt.str();
		return os;
	}

	bool init(){

		return false;
	}

	void 	AddLogFilter 	(std::unique_ptr<LogFilter> filter){
		Logger::GetInstance().AddLogFilter(std::move(filter));
		return;
	}
    void    AddLogTarget    (std::unique_ptr<LogTarget> target){
		Logger::GetInstance().AddLogTarget(std::move(target));
		return;
	}
	bool 	SetLogFilter	(const std::string filter, const std::source_location location){
		if(!Logger::GetInstance().SetLogFilter(filter)){
			throw std::invalid_argument("No filter exists");
		}
		else
			return true;
		return false;
	}
	bool	AddActiveLogTarget	(const std::string target,  const std::source_location location)  {
			return Logger::GetInstance().AddActiveLogTarget(target);
	}
	bool    RemoveActiveLogTarget   (const std::string target,  const std::source_location location){
		return Logger::GetInstance().RemoveActiveLogTarget(target);
	}
	
	LogFilter*  GetLogFilter   (const std::string key){
		return Logger::GetInstance().GetLogFilter(key);
	}
	LogTarget*  GetLogTarget   (const std::string key){
		return Logger::GetInstance().GetLogTarget(key);
	}


	// Logging
	bool 			Log			(const std::string msg, const std::string code, const std::source_location location){
		return Logger::GetInstance().Log(msg, code, location);
	}
	std::string 	ThrowMSG		(const std::string msg, const std::source_location location){
		RSLog::Log(msg, "ERROR", location);
		return msg;
	}
	

}

