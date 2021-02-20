#ifndef ERROR_H
#define ERROR_H

#include <filesystem>
#include <cstdio>
#include <iostream>

#define NO_ERROR 0
#define NO_SUCH_OPTION 1
#define NO_SUCH_POS_OPTION 2
#define NO_SUCH_FILE 3
#define CONFIG_ERROR 4

struct Error : public std::exception {
  Error(int32_t id) : id(id) {}

  int32_t id;
};

struct ArgError : public Error {
  ArgError(int32_t id, const char* arg) : Error(id), arg(arg) {}

  const char* what() const throw() {
    return "Argument Error";
  }

  const char* arg;
};

struct NoOptError : public ArgError {
  NoOptError(const char* arg) : ArgError(NO_SUCH_OPTION, arg) {}

  const char* what() const throw () {
    return "Unknown option";
  }
};

struct UnexpPosOptError : public ArgError {
  UnexpPosOptError(const char* arg) : ArgError(NO_SUCH_POS_OPTION, arg) {}
  const char* what() const throw() {
    return "Unexpected positional argument";
  }
};

struct FileError : public Error {
  FileError(int32_t id, const std::string file) : Error(id), file(file) { }
  const char* what() const throw() {
    return "File Error";
  }

  const std::string file;
};

struct NoSuchFile : public FileError {
  NoSuchFile(const std::string file) : FileError(NO_SUCH_FILE, file) {}
  const char* what() const throw() {
    return "No such file";
  }
};

struct ConfigError : public FileError {
  ConfigError(const std::string file, uint32_t line, uint32_t column, const char* text) : FileError(CONFIG_ERROR, file), line(line), column(column), text(text) {}
  const char* what() const throw() {
    return "Config Parse Error";
  }

  const uint32_t line;
  const uint32_t column;
  const char* text;
};

#endif // ERROR_H
