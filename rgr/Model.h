#pragma once
#include <soci/soci.h>
#include <ctime>

using std::string;

struct Employee { int id; string first, last, email, dept; };
struct Category { int id; string name, desc, type; };
struct Document { int id, emp_id, cat_id; string title, desc; std::tm created, updated; int ver; };
struct File { int id, doc_id; string name, format; int size; std::tm uploaded; };
struct Access { int id, emp_id, doc_id; string level; std::tm granted, expiry; };

struct SearchResult1 { string emp_name, emp_last, doc_title, cat_name; };
struct SearchResult2 { string file_name, format; int size; string doc_title, level; };
struct SearchResult3 { string first, last; long long count; };

class Model {
private:
    soci::session sql;
    long long getLastId(const string& table, const string& id_col);
    void checkDbConnection();
    std::tm stringToTm(const string& date_str);

public:
    Model(const string& conn_string);
    ~Model();

    void printAllEmployees();
    void printAllCategories();
    void printAllDocuments();
    void printAllFiles();
    void printAllAccess();

    void addEmployee(const string& first, const string& last, const string& email, const string& dep);
    void addCategory(const string& name, const string& desc, const string& type);
    void addDocument(int emp_id, int cat_id, const string& title, const string& desc, int version);
    void addFile(int doc_id, const string& name, const string& format, int size);
    void addAccess(int emp_id, int doc_id, const string& level, const string& expiry_str);

    void updateEmployee(int id, const string& first, const string& last, const string& email, const string& dep);
    void updateCategory(int id, const string& name, const string& desc, const string& type); 
    void updateDocument(int id, const string& title, const string& desc, int version);     
    void updateFile(int id, const string& name, const string& format, int size);           
    void updateAccess(int id, const string& level, const string& expiry_str);

    void deleteData(const string& table_name, const string& id_col, int id);

    void generateEmployees(int count);
    void generateCategories(int count);
    void generateDocuments(int count);
    void generateFiles(int count);
    void generateAccess(int count);

    double searchDocsByDeptAndCat(const string& department, const string& category_type);
    double searchFilesByAccessAndSize(const string& access_level, int min_size, int max_size);
    double searchDocCountByEmployeeAndDate(const string& date_start_str, const string& date_end_str);
};