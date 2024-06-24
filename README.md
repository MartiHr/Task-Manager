# Task Manager

Task Manager is a software system for managing tasks, allowing users to create, edit, view, and complete various types of tasks. The system supports user registration and login, management of personal and shared tasks, and tracking and updating task statuses.

## Features

### Task

A task consists of the following attributes:
- **id**: Unique task number.
- **name**: Task name.
- **due_date** (optional): The date by which the task should be completed.
- **status**: Can be ON_HOLD, IN_PROCESS, DONE, OVERDUE.
- **description**: Task description.

**Note**: For a specific due_date, there cannot be more than one task with the same name.

### User

A user of this task management system can be anyone who needs an efficient way to organize and manage their tasks. Users can create, edit, and track various tasks, which can be personal or part of shared projects involving collaboration with other users.

Each user has the following attributes:
- **username**
- **password**

### Dashboard

The dashboard is a panel showing tasks the user wants to complete today. Users can add or remove tasks from the dashboard. Tasks due today are automatically added to the dashboard upon login, and overdue tasks are automatically removed.

## User Functionalities

- **register `<username>` `<password>`**: Register a user with username and password. Registered users are saved in a binary file.
- **login `<username>` `<password>`**: Log in the user.
- **add-task `<name>` `<due_date>` `<description>`**: Add a new task with default status ON_HOLD.
- **update-task-name `<id>` `<name>`**: Change the name of a task.
- **start-task `<id>`**: Mark a task as started.
- **update-task-description `<id>` `<description>`**: Change the description of a task.
- **remove-task-from-dashboard `<id>`**: Remove a task from the dashboard.
- **add-task-to-dashboard `<id>`**: Add a task to the dashboard if it's not overdue.
- **delete-task `<id>`**: Delete a task.
- **get-task `<name>`** or **get-task `<id>`**: Retrieve task information.
- **list-tasks `<date>`**: List all tasks for a specific date.
- **list-tasks**: List all tasks.
- **list-completed-tasks**: List all completed tasks.
- **list-dashboard**: List all tasks on the dashboard.
- **finish-task `<id>`**: Mark a task as finished.
- **logout**: Log out the user.
- **exit**: Exit the program.

**Note**: If a task does not exist when executing a command, an appropriate error message will be displayed.

## Collaboration

A collaboration is a shared project with tasks visible to all participants. Tasks in a collaboration have an additional attribute, `assignee`, indicating the user working on the task. A collaboration has the following attributes:
- **name**: Collaboration name.
- **id**: Unique collaboration number.
- **creator**: User who created the collaboration.
- **workgroup**: Users working on tasks in the collaboration.

### Collaboration Functionalities

- **add-collaboration `<name>`**: Add a new collaboration.
- **delete-collaboration `<name>`**: Delete a collaboration. Only the creator can delete a collaboration, which also removes all tasks in it.
- **list-collaborations**: List all collaborations of the user.
- **add-user `<collaboration name>` `<username>`**: Add a user to a collaboration.
- **assign-task `<collaboration name>` `<username>` `<name>` `<due_date>` `<description>`**: Assign a task to a user in the collaboration.
- **list-tasks `<collaboration name>`**: List all tasks in the collaboration.
