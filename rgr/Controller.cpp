#include "Controller.h"
#include <iostream>  

Controller::Controller(const char* db_connection_string)
    : model(db_connection_string), view()
{
}

void Controller::main_run() {
    bool running = true;
    while (running) {
        try {
            int choice = getMainMenuChoice();
            switch (choice) {
            case 1: handleShowData(); break;
            case 2: handleAddData(); break;
            case 3: handleUpdateData(); break;
            case 4: handleDeleteData(); break;
            case 5: handleGenerateData(); break;
            case 6: handleSearchData(); break;
            case 7: running = false; break;
            default: view.displayStatus(Status::INVALID_CHOICE);
            }
        }
        catch (const std::exception& e) {
            view.displayError(e.what());
        }
        catch (...) {
            view.displayStatus(Status::UNKNOWN_ERROR);
            running = false;
        }
    }
}

int Controller::getMainMenuChoice() { view.displayMainMenu(); return view.getIntInput("Your choice: "); }
int Controller::getShowMenuChoice() { view.displayShowMenu(); return view.getIntInput("Your choice: "); }
int Controller::getAddMenuChoice() { view.displayAddMenu(); return view.getIntInput("Your choice: "); }
int Controller::getUpdateMenuChoice() { view.displayUpdateMenu(); return view.getIntInput("Your choice: "); }
int Controller::getGenerateMenuChoice() { view.displayGenerateMenu(); return view.getIntInput("Your choice: "); }
int Controller::getSearchMenuChoice() { view.displaySearchMenu(); return view.getIntInput("Your choice: "); }

void Controller::handleShowData() {
    bool running = true;
    while (running) {
        int choice = getShowMenuChoice();
        switch (choice) {
        case 1:
            view.displaySectionTitle("1. ACCESS");
            model.printAllAccess();
            break;
        case 2:
            view.displaySectionTitle("2. CATEGORIES");
            model.printAllCategories();
            break;
        case 3:
            view.displaySectionTitle("3. DOCUMENTS");
            model.printAllDocuments();
            break;
        case 4:
            view.displaySectionTitle("4. EMPLOYEES");
            model.printAllEmployees();
            break;
        case 5:
            view.displaySectionTitle("5. FILES");
            model.printAllFiles();
            break;
        case 6:
            view.displaySectionTitle("1. ACCESS");
            model.printAllAccess();
            view.displaySectionTitle("2. CATEGORIES");
            model.printAllCategories();
            view.displaySectionTitle("3. DOCUMENTS");
            model.printAllDocuments();
            view.displaySectionTitle("4. EMPLOYEES");
            model.printAllEmployees();
            view.displaySectionTitle("5. FILES");
            model.printAllFiles();
            break;
        case 7:
            running = false;
            break;
        default:
            view.displayStatus(Status::INVALID_CHOICE);
        }
    }
}

void Controller::handleAddData() {
    bool running = true;
    while (running) {
        int choice = getAddMenuChoice();
        bool added = false;
        switch (choice) {
        case 1: handleAddAccess(); added = true; break;
        case 2: handleAddCategory(); added = true; break;
        case 3: handleAddDocument(); added = true; break;
        case 4: handleAddEmployee(); added = true; break;
        case 5: handleAddFile(); added = true; break;
        case 6: running = false; break;
        default: view.displayStatus(Status::INVALID_CHOICE);
        }
        if (added && running) {
            view.displayStatus(Status::INPUT_SUCCESS);
        }
    }
}

void Controller::handleUpdateData() {
    bool running = true;
    while (running) {
        int choice = getUpdateMenuChoice();
        bool updated = false;
        switch (choice) {
        case 1: handleUpdateAccess(); updated = true; break;
        case 2: handleUpdateCategory(); updated = true; break;
        case 3: handleUpdateDocument(); updated = true; break;
        case 4: handleUpdateEmployee(); updated = true; break;
        case 5: handleUpdateFile(); updated = true; break;
        case 6: running = false; break;
        default: view.displayStatus(Status::INVALID_CHOICE);
        }
        if (updated && running) {
            view.displayStatus(Status::UPDATE_SUCCESS);
        }
    }
}

void Controller::handleDeleteData() {
    view.displaySectionTitle("4. DELETE DATA");
    std::string table = view.getInput("Enter table name (e.g., employee): ");
    std::string db_table = (table == "category") ? "category" : table;
    std::string id_col = view.getInput("Enter ID column name (e.g., employee_id): ");
    int id = view.getIntInput("Enter ID to delete: ");
    model.deleteData(db_table, id_col, id);
    view.displayStatus(Status::DELETE_SUCCESS);
}

int Controller::getGenerationCount() {
    int n = view.getIntInput("Number to generate: ");
    if (n <= 0) {
        view.displayStatus(Status::INVALID_INPUT_GENERAL, "Please enter a positive number.");
        return 0;
    }
    return n;
}

void Controller::handleGenerateData() {
    bool running = true;
    while (running) {
        int choice = getGenerateMenuChoice();
        switch (choice) {
        case 1: handleGenerateAccess(); break;
        case 2: handleGenerateCategories(); break;
        case 3: handleGenerateDocuments(); break;
        case 4: handleGenerateEmployees(); break;
        case 5: handleGenerateFiles(); break;
        case 6: running = false; break;
        default: view.displayStatus(Status::INVALID_CHOICE);
        }
    }
}

void Controller::handleSearchData() {
    bool running = true;
    while (running) {
        int choice = getSearchMenuChoice();
        switch (choice) {
        case 1: handleSearch1(); break;
        case 2: handleSearch2(); break;
        case 3: handleSearch3(); break;
        case 4: running = false; break;
        default: view.displayStatus(Status::INVALID_CHOICE);
        }
    }
}

void Controller::handleAddEmployee() {
    std::string first = view.getInput("Enter first name: ");
    std::string last = view.getInput("Enter last name: ");
    std::string email = view.getInput("Enter email: ");
    std::string dep = view.getInput("Enter department: ");
    model.addEmployee(first, last, email, dep);
}
void Controller::handleAddCategory() {
    std::string name = view.getInput("Enter category name: ");
    std::string desc = view.getInput("Enter description: ");
    std::string type = view.getInput("Enter type: ");
    model.addCategory(name, desc, type);
}
void Controller::handleAddDocument() {
    int emp_id = view.getIntInput("Enter Author's Employee ID: ");
    int cat_id = view.getIntInput("Enter Category ID: ");
    std::string title = view.getInput("Enter title: ");
    std::string desc = view.getInput("Enter description: ");
    int ver = view.getIntInput("Enter version: ");
    model.addDocument(emp_id, cat_id, title, desc, ver);
}
void Controller::handleAddFile() {
    int doc_id = view.getIntInput("Enter Document ID: ");
    std::string name = view.getInput("Enter file name: ");
    std::string format = view.getInput("Enter format: ");
    int size = view.getIntInput("Enter size (KB): ");
    model.addFile(doc_id, name, format, size);
}
void Controller::handleAddAccess() {
    int emp_id = view.getIntInput("Enter Employee ID: ");
    int doc_id = view.getIntInput("Enter Document ID: ");
    std::string level = view.getInput("Enter access level: ");
    std::string expiry = view.getInput("Enter expiry date (YYYY-MM-DD): ");
    model.addAccess(emp_id, doc_id, level, expiry);
}
void Controller::handleUpdateEmployee() {
    int id = view.getIntInput("Enter Employee ID to update: ");
    std::string first = view.getInput("Enter new first name: ");
    std::string last = view.getInput("Enter new last name: ");
    std::string email = view.getInput("Enter new email: ");
    std::string dep = view.getInput("Enter new department: ");
    model.updateEmployee(id, first, last, email, dep);
}

void Controller::handleUpdateCategory() {
    int id = view.getIntInput("Enter Category ID to update: ");
    std::string name = view.getInput("Enter new name: ");
    std::string desc = view.getInput("Enter new description: ");
    std::string type = view.getInput("Enter new type: ");
    model.updateCategory(id, name, desc, type);
}

void Controller::handleUpdateDocument() {
    int id = view.getIntInput("Enter Document ID to update: ");
    std::string title = view.getInput("Enter new title: ");
    std::string desc = view.getInput("Enter new description: ");
    int ver = view.getIntInput("Enter new version: ");
    model.updateDocument(id, title, desc, ver);
}

void Controller::handleUpdateFile() {
    int id = view.getIntInput("Enter File ID to update: ");
    std::string name = view.getInput("Enter new file name: ");
    std::string format = view.getInput("Enter new format: ");
    int size = view.getIntInput("Enter new size (KB): ");
    model.updateFile(id, name, format, size);
}

void Controller::handleUpdateAccess() {
    int id = view.getIntInput("Enter Access ID to update: ");
    std::string level = view.getInput("Enter new access level: ");
    std::string expiry = view.getInput("Enter new expiry date (YYYY-MM-DD): ");
    model.updateAccess(id, level, expiry);
}

void Controller::handleGenerateEmployees() {
    int n = getGenerationCount(); if (n == 0) return;
    model.generateEmployees(n);
    std::ostringstream oss; oss << n;
    view.displayStatus(Status::GENERATE_SUCCESS, oss.str());
}
void Controller::handleGenerateCategories() {
    int n = getGenerationCount(); if (n == 0) return;
    model.generateCategories(n);
    std::ostringstream oss; oss << n;
    view.displayStatus(Status::GENERATE_SUCCESS, oss.str());
}
void Controller::handleGenerateDocuments() {
    int n = getGenerationCount(); if (n == 0) return;
    model.generateDocuments(n);
    std::ostringstream oss; oss << n;
    view.displayStatus(Status::GENERATE_SUCCESS, oss.str());
}
void Controller::handleGenerateFiles() {
    int n = getGenerationCount(); if (n == 0) return;
    model.generateFiles(n);
    std::ostringstream oss; oss << n;
    view.displayStatus(Status::GENERATE_SUCCESS, oss.str());
}
void Controller::handleGenerateAccess() {
    int n = getGenerationCount(); if (n == 0) return;
    model.generateAccess(n);
    std::ostringstream oss; oss << n;
    view.displayStatus(Status::GENERATE_SUCCESS, oss.str());
}

void Controller::handleSearch1() {
    std::string dept = view.getInput("Enter department (like): ");
    std::string type = view.getInput("Enter category type (like): ");
    double time = model.searchDocsByDeptAndCat(dept, type);
    view.showExecutionTime(time);
}
void Controller::handleSearch2() {
    int min_s = view.getIntInput("Enter MIN file size (KB): ");
    int max_s = view.getIntInput("Enter MAX file size (KB): ");
    if (min_s > max_s) { view.displayStatus(Status::INVALID_INPUT_GENERAL, "Min size cannot be greater than max size."); return; }
    std::string level = view.getInput("Enter access level (like): ");
    double time = model.searchFilesByAccessAndSize(level, min_s, max_s);
    view.showExecutionTime(time);
}
void Controller::handleSearch3() {
    std::string start = view.getInput("Enter start date (YYYY-MM-DD): ");
    std::string end = view.getInput("Enter end date (YYYY-MM-DD): ");
    double time = model.searchDocCountByEmployeeAndDate(start, end);
    view.showExecutionTime(time);
}