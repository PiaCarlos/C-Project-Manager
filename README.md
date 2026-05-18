# C++ Project Manager

This project is a simple project management system written in C++.

It manages projects, team members, tasks, task assignments, task status, and task filtering. The program is run from the command line and demonstrates object-oriented programming and dynamic memory management in C++.

## Features

- Create projects
- Add team members with roles and maximum task limits
- Create tasks with descriptions, priorities, and statuses
- Assign tasks to team members
- Prevent team members from receiving more than their maximum number of tasks
- Display all projects and their tasks
- Display all team members and their assigned tasks
- Find a project by ID
- Find a team member by ID
- Get the assigned member for a task
- Get all tasks assigned to a specific member
- Filter tasks by status
- Test deep copying with a custom copy constructor

## Concepts Used

- Classes and objects
- Constructors and destructors
- Copy constructor
- Dynamic memory allocation
- Arrays of pointers
- Encapsulation
- Getter methods
- Pass-by-reference
- Object-oriented design

## How to Run

Compile the program:

```bash
g++ project_Manager.cpp -o project_manager
