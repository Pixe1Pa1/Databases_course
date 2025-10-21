# Calculation and-Graphical Work (RGR)

## Short Description
**Topic**: Creating a database application focused on interacting with the PostgreSQL database. (Development of a C++ console application for managing a PostgreSQL database using the MVC pattern and the SOCI library.)
**Purpose**: acquiring skills in programming PostgreSQL database applications.

## Key Tasks
  1. Implement functions for viewing, adding, editing, and deleting data in PostgreSQL database tables.
  2. Implement automatic batch generation of randomized data in the database using SQL functions.
  3. Provide search functionality across multiple attributes from two or more tables (supporting ranges for numbers, LIKE patterns for strings, and GROUP BY).
  4. Organize the program code according to the MVC pattern, separating logic, presentation, and control into distinct modules.

## Project Files
- **Controller.h, Controller.cpp** - The Controller module. Manages the application flow, linking the View and Model.
- **Model.h, Model.cpp** - The Model module. Contains all business logic and SQL queries for interacting with the database via SOCI.
- **main.cpp** - The application's entry point. Initializes the Controller and View.
- **rgr.pdf** - The complete report for the calculation-and-graphical work.
- **RGR_DB/x64/Debug** - Directory containing the project's build artifacts.

## How to Deploy and Build
This project was developed in C++ using Visual Studio 2022. It uses the SOCI C++ library to interact with PostgreSQL.
To build the project correctly, you must install the SOCI library and its dependencies (specifically, libpq for PostgreSQL). The easiest way to do this is by using the vcpkg package manager.

1. **Install vcpkg:** First, you need to install the vcpkg package manager. This tool automatically downloads, compiles, and integrates libraries into Visual Studio. A detailed installation guide can be found in this video: https://youtu.be/0h1lC3QHLHU?si=1OtInhPu8mZ8n1_P
2. **Install the SOCI library (with PostgreSQL support):** After setting up vcpkg, open a terminal (PowerShell) in the vcpkg directory and execute the following commands:

- Search for the required package:
  .\vcpkg.exe search soci
- Install SOCI with the PostgreSQL backend feature for 64-bit Windows:
  .\vcpkg.exe install soci[postgresql]:x64-windows
Using it in the project: After this, Visual Studio will automatically detect the installed library (due to vcpkg integration), and you will be able to use #include <soci/soci.h> in your project.
