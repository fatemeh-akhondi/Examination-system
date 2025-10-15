# Examination System

A console-based C++ application for managing and conducting exams.  
Supports **student** and **professor** roles, multiple question types, grading, and CSV result export.

---

## Features

- User authentication (students & professors)  
- Create exams with various question types  
  - Multiple Choice (4 options)  
  - Short answer  
  - Long / Full answer  
- Conduct exams from CLI  
- Automatic grading and manual review  
- Export results as CSV  
- Access data of previous sessions

## Project structure

├── core/ # Core domain logic (Exam, Question, Member etc.)
├── communication/ # CLI interfaces for student, professor, auth
├── data_exporter/ # CSV export logic
├── data_loader/ # Loading previous data / persistence
├── exceptions/ # Custom exception classes
├── utils/ # Utility helpers and common tools
├── main.cpp # Entry point

## Build

Run the following command inside src folder:
```bash
make
```
Then run:
```bash
./exam_system
```
