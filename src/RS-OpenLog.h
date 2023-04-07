#ifndef RS_OPEN_LOG
#define RS_OPEN_LOG

#include <source_location>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <chrono>



namespace RSLog{

    struct LogData{
        const std::string 											m_msg;
        const std::string										    m_code;
        const std::source_location									m_location;
        const std::chrono::time_point<std::chrono::system_clock> 	m_timestamp;
    };
    struct LogSettings{ 
        bool m_showTime 		        { true };
        bool m_showCode 		        { true };
        bool m_showMsg			        { true };
        bool m_showLocation 	        { true };
        uint8_t m_widthOfCodeTextBox 	{ 7 };
        uint8_t m_logMsgMaxSize			{ 64 };
    };

    class LogCode {
    public:
        LogCode(const std::string code);
        virtual ~LogCode();

        virtual const std::string str() const noexcept;
        friend std::ostream& operator<<(std::ostream& os, const LogCode& lc);

    private:
        std::string m_code { "???" };
    };

    class LogFilter {
    public:
        LogFilter    (const std::string name);
        virtual ~LogFilter   ();

        void AddLogCode		( LogCode* lc );
        void AddLogCode		( std::unique_ptr<LogCode> lc );

        bool RemoveLogCode  ( const std::string key  );
        bool IsAllowed      ( const std::string logCodeKey  ) const;

        virtual const std::string str() const noexcept;
        friend std::ostream& operator<<(std::ostream& os, const LogFilter& lc);

    private:
        std::string m_name { "???" };
        std::unordered_map<std::string, std::unique_ptr<LogCode>> m_logCodesAllowed;
    };
    
    class LogTarget{
    public:
        LogTarget(const std::string name);
        virtual ~LogTarget();
    
        virtual const std::string str() const noexcept;        
        virtual bool Log (const LogData& log,  const LogSettings& settings);

        friend std::ostream& operator<<(std::ostream& os, const LogFilter& lc);

    private:
        const std::string m_name {"NULL"};
    };

    bool        init();

    bool        SetLogFilter            (const std::string filter,  const std::source_location location=std::source_location::current());
    bool        AddActiveLogTarget	    (const std::string target,  const std::source_location location=std::source_location::current());
    bool        RemoveActiveLogTarget   (const std::string target,  const std::source_location location=std::source_location::current());

    void        AddLogFilter    (std::unique_ptr<LogFilter> filter);
    void        AddLogTarget    (std::unique_ptr<LogTarget> target);

    LogFilter*  GetLogFilter    (const std::string key);
    LogTarget*  GetLogTarget    (const std::string key);

    // Logging
    bool        Log             (const std::string msg, std::string code,   const std::source_location location=std::source_location::current());
    std::string ThrowMSG        (const std::string msg,                     const std::source_location location=std::source_location::current());

}


#endif