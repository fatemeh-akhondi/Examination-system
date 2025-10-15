# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Target executable
TARGET = exam_system

# Source files
SRCS = main.cpp \
       core/instance_manager.cpp \
       core/question.cpp \
       core/four_multiple_choice.cpp \
       core/long_answer.cpp \
       core/short_answer.cpp \
       core/exam_response.cpp \
       core/exam.cpp \
       core/member.cpp \
       core/professor.cpp \
       core/student.cpp \
       communication/authentication_cli.cpp \
       communication/exam_cli.cpp \
       communication/professor_cli.cpp \
       communication/question_creator.cpp \
       communication/student_cli.cpp \
       utils/tools.cpp \
       exceptions/exception.cpp \
       data_exporter/data_exporter.cpp \
       data_loader/data_loader.cpp \

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)