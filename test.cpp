/*
 * test.cpp
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
 
 using namespace chreosis;

int main() {
	Logger::instance()->setGlobalSeverity(chreosis::Logger::LogSeverity::LogAlert);
	Logger::instance()->AddLogFileOutputStream("logfile1.log", true);
	Logger::instance()->AddLogFileOutputStream("logfile2.log", false);
	Logger::instance()->AddOutputStream(std::cerr);
	Logger::instance()->AddOutputStream(std::cout);
	// Without Macro
	chreosis::Logger::instance()->Log(chreosis::Logger::LogEmergency, STRSTREAM("whoo" << 123 << "fdsf"),__FILE__, __LINE__, __FUNCTION__ );
	
	// With Macro
	LOG(chreosis::Logger::LogError, "Whooe" << " this" << " is " << "Awesome!!");
	LOG_ERROR("Error occured");
	int i = 0;
	while(i < 50) {
		LOG_ALERT_N(5, "every 5 times");
		i++;
	}
	return 0;
 }