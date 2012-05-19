/*
 * logger.hpp
 *
 *  Created on: 12 Apr 2012
 *      Author: Climax
 *
 Copyright 2012 Pieter Jordaan

 Distributed under the GNU Lesser General Public License

 This file is part of MyLogger.

 MyLogger is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MyLogger is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with MyLogger.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
namespace chreosis {
class LogBackend: public boost::enable_shared_from_this<LogBackend> {
public:
	LogBackend(ostream* stream = 0) :
			m_stream(stream), m_verbosity_level(0), m_always_flush(false) {
	}
	virtual void Flush() {
		m_stream->flush();
	}
	virtual ~LogBackend() {
		if (m_stream)
			m_stream->flush();
	}
protected:
	ostream* m_stream;
	unsigned int m_verbosity_level;
	bool m_always_flush;
	friend class Logger;

public:
	bool isAlwaysFlush() const {
		return m_always_flush;
	}

	void setAlwaysFlush(bool alwaysFlush) {
		m_always_flush = alwaysFlush;
	}

	unsigned int getVerbosityLevel() const {
		return m_verbosity_level;
	}

	void setVerbosityLevel(unsigned int verbosityLevel) {
		m_verbosity_level = verbosityLevel;
	}
};

class FileLogBackend: public LogBackend {
public:
	FileLogBackend(string filename, bool append = false) :
			LogBackend() {
		m_filename = filename;
		m_always_flush = true;
		m_file_stream = new ofstream;
		m_stream = (ostream*) m_file_stream;
		m_file_stream->open(filename.c_str(),
				((append) ? ios_base::app : (ios_base::openmode) 0)
						| ios_base::out);
	}
	virtual void Flush() {
		m_file_stream->flush();
	}
	virtual ~FileLogBackend() {
		if (m_file_stream) {
			if (m_file_stream->is_open()) {
				m_file_stream->flush();
				m_file_stream->close();
			}
			delete m_file_stream;
		}

	}

private:
	string m_filename;
	ofstream* m_file_stream;

	friend class Logger;
};

#define STRSTREAM(input) (((stringstream*)(&(stringstream() << input)))->str())
class Logger {
public:
	enum LogSeverity {
		LogEmergency = 0,
		LogAlert,
		LogCritical,
		LogError,
		LogWarning,
		LogNotice,
		LogInfo,
		LogDebug,
		LogAll = 0xFFFFFFFF
	};
	static string LogSeverityToString(LogSeverity severity) {
		switch (severity) {
		case LogEmergency:
			return "EMERGENCY!!!";
		case LogAlert:
			return "ALERT!!";
		case LogCritical:
			return "CRITICAL!";
		case LogError:
			return "Error!";
		case LogWarning:
			return "Warning";
		case LogNotice:
			return "Note";
		case LogInfo:
			return "Info";
		case LogDebug:
			return "Debug";
		default:
			return "Other";
		}
	}
	static Logger *instance() {
		if (!s_instance)
			s_instance = new Logger;
		return s_instance;
	}
	boost::shared_ptr<LogBackend> AddOutputStream(ostream& stream,
			LogSeverity severity = LogAll);
	void RemoveOutputStream(ostream& stream);
	boost::shared_ptr<LogBackend> AddLogFileOutputStream(string filename,
			bool append = true, LogSeverity severity = LogAll);

	inline void AlwaysFlushStreams(bool enable) {
		m_always_flush = enable;
		if (enable)
			FlushAllStreams();
	}
	void FlushAllStreams();

	void Log(LogSeverity severity, string message, string file, int line,
			string function);

	LogSeverity getGlobalSeverity() const {
		return m_global_severity;
	}

	void setGlobalSeverity(LogSeverity globalSeverity) {
		m_global_severity = globalSeverity;
	}
private:

	static Logger *s_instance;
	Logger() {
		m_always_flush = false;
		m_global_severity = LogAll;
	}

	bool m_always_flush;
	LogSeverity m_global_severity;
	list<boost::shared_ptr<LogBackend>> m_output_streams;
};
}

#define MYLOG(severity, message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)severity, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_N(modulus, severity, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log(((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogSeverity)severity, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_EMERGENCY(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogEmergency, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_EMERGENCY_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogEmergency, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_ALERT(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogAlert, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_ALERT_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogAlert, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_CRITICAL(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogCritical, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_CRITICAL_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogCritical, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_ERROR(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogError, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_ERROR_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogError, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_WARNING(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogWarning, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_WARNING_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogWarning, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_NOTICE(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogNotice, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_NOTICE_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogNotice, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#define MYLOG_INFO(message) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogInfo, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_INFO_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log((chreosis::Logger::LogSeverity)chreosis::Logger::LogSeverity::LogInfo, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}

#ifdef _DEBUG
#define MYLOG_DEBUG(message) chreosis::Logger::instance()->Log(chreosis::Logger::LogSeverity::LogDebug, STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__)
#define MYLOG_DEBUG_N(modulus, message) {static unsigned int counter = 0; counter ++; if(counter % modulus == 0) chreosis::Logger::instance()->Log(chreosis::Logger::LogDebug STRSTREAM(message), __FILE__, __LINE__, __FUNCTION__);}
#else
#define MYLOG_DEBUG(message)
#define MYLOG_DEBUG_N(modulus, message)
#endif

#endif /* LOGGER_HPP_ */
