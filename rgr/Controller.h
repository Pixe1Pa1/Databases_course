#pragma once
#include "Model.h"
#include "View.h"

class Controller {
public:
    Controller(const char* db_connection_string);
    void main_run();

private:
    Model model;
    View view;

    int getMainMenuChoice();
    int getShowMenuChoice();
    int getAddMenuChoice();
    int getUpdateMenuChoice();
    int getGenerateMenuChoice();
    int getSearchMenuChoice();

    void handleShowData();
    void handleAddData();
    void handleUpdateData();
    void handleDeleteData();
    void handleGenerateData();
    void handleSearchData();

    void handleAddEmployee();
    void handleAddCategory(); 
    void handleAddDocument();
    void handleAddFile(); 
    void handleAddAccess();

    void handleUpdateEmployee();
    void handleUpdateCategory();   
    void handleUpdateDocument();  
    void handleUpdateFile();        
    void handleUpdateAccess();


    void handleGenerateEmployees(); 
    void handleGenerateCategories(); 
    void handleGenerateDocuments();
    void handleGenerateFiles(); 
    void handleGenerateAccess();

    void handleSearch1(); 
    void handleSearch2(); 
    void handleSearch3();

    int getGenerationCount();
};