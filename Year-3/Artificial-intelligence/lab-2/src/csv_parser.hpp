#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>

#pragma once

#define CSV_DELIMETER ';'

class CSVParser {
private:
	std::ifstream m_file;
	char m_delimeter{};

public:
	explicit CSVParser(const std::string & file_name)
			: m_file(std::ifstream(file_name)) {}

	~CSVParser() { m_file.close(); }

	template<typename... Args>
	std::vector<std::tuple<Args...>> parse_file(char delimeter = CSV_DELIMETER) {
		// pass delimeter
		m_delimeter = delimeter;
		std::vector<std::tuple<Args...>> data;
		std::string line;
		while (m_file >> line) {
			std::stringstream ss(line);
			data.push_back(parse_line<Args...>(ss));
		}
		return data;
	}

private:
	template<typename... Args>
	std::tuple<Args...> parse_line(std::stringstream & line_stream) {
		return std::tuple<Args...>{read_argument<Args>(line_stream)...};
	}

	template<typename T,
					 std::enable_if_t<!std::is_convertible_v<T, std::string>, bool> = true>
	T read_argument(std::stringstream & line_stream) {
		T arg;
		line_stream >> arg;
		return arg;
	}

	template<typename T,
					 std::enable_if_t<std::is_convertible_v<T, std::string>, bool> = true>
	T read_argument(std::stringstream & line_stream) {
		T string;
		getline(line_stream, string, m_delimeter);
		return string;
	}
};// CSVParser

#undef CSV_DELIMETER
