/*
 * logger.cpp
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

#include "logger.hpp"
#include <iostream>
#include <algorithm>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/date_time.hpp>

using namespace std;
namespace chreosis {
Logger* Logger::s_instance = 0;
boost::shared_ptr<LogBackend> Logger::AddOutputStream(ostream& stream,
		LogSeverity severity) {
	boost::shared_ptr<LogBackend> backend(new LogBackend(&stream));
	m_output_streams.push_back(backend);
	backend->m_verbosity_level = (unsigned int) severity;
	return backend;
}

void Logger::RemoveOutputStream(ostream& stream) {
	m_output_streams.remove_if(
			[&stream](const boost::shared_ptr<LogBackend>& backend)
			{	return backend->m_stream == stream;});
}

boost::shared_ptr<LogBackend> Logger::AddLogFileOutputStream(string filename,
		bool append, LogSeverity severity) {
	boost::shared_ptr<LogBackend> backend(
			(LogBackend*) (new FileLogBackend(filename, append)));
	m_output_streams.push_back(backend);
	backend->m_verbosity_level = (unsigned int) severity;
	return backend;
}

void Logger::FlushAllStreams() {
	for (list<boost::shared_ptr<LogBackend> >::iterator iter =
			m_output_streams.begin(); iter != m_output_streams.end(); ++iter) {
		iter->get()->Flush();
	}
}

void Logger::Log(LogSeverity severity, string message, string file, int line,
		string function) {

	if (m_global_severity >= severity) {
		stringstream ss;
		ss << "["
				<< boost::posix_time::to_iso_extended_string(
						boost::posix_time::microsec_clock::local_time())
				<< "] @ " << function << " in " << file << "(" << line << ") - "
				<< " " << LogSeverityToString(severity) << " " << message
				<< endl;
		string logentry = ss.str();
		for (list<boost::shared_ptr<LogBackend> >::iterator iter =
				m_output_streams.begin(); iter != m_output_streams.end();
				++iter) {
			if (iter->get()->m_verbosity_level >= (unsigned int) severity) {
				*iter->get()->m_stream << logentry;
				if (m_always_flush || iter->get()->m_always_flush)
					iter->get()->Flush();
			}
		}
	}
}
}
