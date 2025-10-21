#include "Model.h"
#include <soci/postgresql/soci-postgresql.h>
#include <soci/rowset.h>
#include <iostream>
#include <ctime>
#include <iomanip>

namespace soci
{
     template<> struct type_conversion<Employee> {
        typedef values base_type;
        static void from_base(values const& v, indicator, Employee& e) {
            e.id = v.get<int>("employee_id"); e.first = v.get<string>("first_name");
            e.last = v.get<string>("last_name"); e.email = v.get<string>("email");
            e.dept = v.get<string>("department");
        }
    };
    template<> struct type_conversion<Category> {
        typedef values base_type;
        static void from_base(values const& v, indicator, Category& c) {
            c.id = v.get<int>("category_id"); c.name = v.get<string>("category_name");
            c.desc = v.get<string>("description"); c.type = v.get<string>("type");
        }
    };
    template<> struct type_conversion<Document> {
        typedef values base_type;
        static void from_base(values const& v, indicator, Document& d) {
            d.id = v.get<int>("document_id"); d.emp_id = v.get<int>("employee_id");
            d.cat_id = v.get<int>("category_id"); d.title = v.get<string>("title");
            d.desc = v.get<string>("description"); d.created = v.get<std::tm>("date_created");
            d.updated = v.get<std::tm>("date_updated"); d.ver = v.get<int>("version");
        }
    };
    template<> struct type_conversion<File> {
        typedef values base_type;
        static void from_base(values const& v, indicator, File& f) {
            f.id = v.get<int>("file_id"); f.doc_id = v.get<int>("document_id");
            f.name = v.get<string>("file_name"); f.format = v.get<string>("format");
            f.size = v.get<int>("size"); f.uploaded = v.get<std::tm>("upload_date");
        }
    };
    template<> struct type_conversion<Access> {
        typedef values base_type;
        static void from_base(values const& v, indicator, Access& a) {
            a.id = v.get<int>("access_id"); a.emp_id = v.get<int>("employee_id");
            a.doc_id = v.get<int>("document_id"); a.level = v.get<string>("access_level");
            a.granted = v.get<std::tm>("date_granted"); a.expiry = v.get<std::tm>("expiry_date");
        }
    };
}

static std::string truncate(std::string str, size_t width) {
    if (str.length() > width) {
        return str.substr(0, width - 3) + "...";
    }
    return str;
}


Model::Model(const string& conn_string) {
    try {
        sql.open(soci::postgresql, conn_string);
        checkDbConnection();
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Database connection failed: " + string(e.what()));
    }
}

Model::~Model() {
    if (sql.is_connected()) { sql.close(); }
}

void Model::checkDbConnection() {
    if (!sql.is_connected()) { throw std::runtime_error("Database connection lost."); }
}

long long Model::getLastId(const string& table, const string& id_col) {
    long long last_id = 0;
    sql << "SELECT COALESCE(MAX(\"" + id_col + "\"), 0) FROM public.\"" + table + "\"", soci::into(last_id);
    return last_id;
}

std::tm Model::stringToTm(const string& date_str) {
    std::tm t = {};
    if (date_str.length() != 10 || date_str[4] != '-' || date_str[7] != '-') {
        throw std::runtime_error("ERROR: Invalid date format. Use YYYY-MM-DD.");
    }
    try {
        t.tm_year = std::stoi(date_str.substr(0, 4));
        t.tm_mon = std::stoi(date_str.substr(5, 2));
        t.tm_mday = std::stoi(date_str.substr(8, 2));
    }
    catch (...) {
        throw std::runtime_error("ERROR: Invalid date components. Use YYYY-MM-DD.");
    }
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return t;
}

string tmToStringHelper(std::tm t) {
    char buffer[11];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &t)) {
        return string(buffer);
    }
    return "invalid date";
}

void Model::printAllEmployees() {
    checkDbConnection();
    soci::rowset<Employee> rs = (sql.prepare << "SELECT * FROM public.employee ORDER BY employee_id");
    int count = 0;

    std::cout << std::left
        << std::setw(6) << "ID" << " | "
        << std::setw(30) << "Name" << " | "
        << std::setw(35) << "Email" << " | "
        << std::setw(30) << "Department" << "\n"; 

    std::cout << string(109, '-') << "\n"; 

    for (const auto& e : rs) {
        std::cout << std::left
            << std::setw(6) << e.id << " | "
            << std::setw(30) << truncate(e.first + " " + e.last, 30) << " | "
            << std::setw(35) << truncate(e.email, 35) << " | "
            << std::setw(30) << truncate(e.dept, 30) << "\n"; 
        count++;
    }
    if (count == 0) std::cout << "No employees found.\n";
}
void Model::printAllCategories() {
    checkDbConnection();
    soci::rowset<Category> rs = (sql.prepare << "SELECT * FROM public.category ORDER BY category_id");
    int count = 0;

    std::cout << std::left
        << std::setw(6) << "ID" << " | "
        << std::setw(30) << "Name" << " | "
        << std::setw(40) << "Description" << " | "
        << std::setw(15) << "Type" << "\n";
    std::cout << string(100, '-') << "\n";

    for (const auto& c : rs) {
        std::cout << std::left
            << std::setw(6) << c.id << " | "
            << std::setw(30) << truncate(c.name, 30) << " | "
            << std::setw(40) << truncate(c.desc, 40) << " | "
            << std::setw(15) << truncate(c.type, 15) << "\n";
        count++;
    }
    if (count == 0) std::cout << "No categories found.\n";
}

void Model::printAllDocuments() {
    checkDbConnection();
    soci::rowset<Document> rs = (sql.prepare << "SELECT * FROM public.document ORDER BY document_id");
    int count = 0;

    for (const auto& d : rs) {
        std::cout << "--------------------------------------------------------------------------------\n"
            << std::left
            << std::setw(6) << "ID: " << std::setw(10) << d.id
            << "| Title: " << truncate(d.title, 50)
            << " | V: " << d.ver << "\n"
            << "  Desc: " << truncate(d.desc, 65) << "\n"
            << "  " << std::setw(13) << ("AuthorID: " + std::to_string(d.emp_id))
            << "| " << std::setw(10) << ("CatID: " + std::to_string(d.cat_id)) << "\n"
            << "  " << std::setw(22) << ("Created: " + tmToStringHelper(d.created))
            << "| Updated: " << tmToStringHelper(d.updated) << "\n";
        count++;
    }
    if (count == 0) std::cout << "No documents found.\n";
}

void Model::printAllFiles() {
    checkDbConnection();
    soci::rowset<File> rs = (sql.prepare << "SELECT * FROM public.file ORDER BY file_id");
    int count = 0;

    std::cout << std::left
        << std::setw(6) << "ID" << " | "
        << std::setw(35) << "Name" << " | "
        << std::setw(10) << "Format" << " | "
        << std::setw(10) << "Size(KB)" << " | "
        << std::setw(8) << "DocID" << " | "
        << std::setw(12) << "Uploaded" << "\n";
    std::cout << string(95, '-') << "\n";

    for (const auto& f : rs) {
        std::cout << std::left
            << std::setw(6) << f.id << " | "
            << std::setw(35) << truncate(f.name, 35) << " | "
            << std::setw(10) << truncate(f.format, 10) << " | "
            << std::setw(10) << f.size << " | "
            << std::setw(8) << f.doc_id << " | "
            << std::setw(12) << tmToStringHelper(f.uploaded) << "\n";
        count++;
    }
    if (count == 0) std::cout << "No files found.\n";
}

void Model::printAllAccess() {
    checkDbConnection();
    soci::rowset<Access> rs = (sql.prepare << "SELECT * FROM public.access ORDER BY access_id");
    int count = 0;

    std::cout << std::left
        << std::setw(6) << "ID" << " | "
        << std::setw(8) << "EmpID" << " | "
        << std::setw(8) << "DocID" << " | "
        << std::setw(15) << "Access Level" << " | "
        << std::setw(12) << "Granted" << " | "
        << std::setw(12) << "Expires" << "\n";
    std::cout << string(84, '-') << "\n";

    for (const auto& a : rs) {
        std::cout << std::left
            << std::setw(6) << a.id << " | "
            << std::setw(8) << a.emp_id << " | "
            << std::setw(8) << a.doc_id << " | "
            << std::setw(15) << truncate(a.level, 15) << " | "
            << std::setw(12) << tmToStringHelper(a.granted) << " | "
            << std::setw(12) << tmToStringHelper(a.expiry) << "\n";
        count++;
    }
    if (count == 0) std::cout << "No access found.\n";
}

void Model::addEmployee(const string& first, const string& last, const string& email, const string& dep) {
    checkDbConnection();
    long long new_id = getLastId("employee", "employee_id") + 1;
    sql << "INSERT INTO public.employee (employee_id, first_name, last_name, email, department) VALUES (:id, :f, :l, :e, :d)",
        soci::use(new_id), soci::use(first), soci::use(last), soci::use(email), soci::use(dep);
}

void Model::addCategory(const string& name, const string& desc, const string& type) {
    checkDbConnection();
    long long new_id = getLastId("category", "category_id") + 1;
    sql << "INSERT INTO public.category (category_id, category_name, description, type) VALUES (:id, :n, :d, :t)",
        soci::use(new_id), soci::use(name), soci::use(desc), soci::use(type);
}

void Model::addDocument(int emp_id, int cat_id, const string& title, const string& desc, int version) {
    checkDbConnection();
    long long new_id = getLastId("document", "document_id") + 1;
    std::tm now = {}; time_t t = time(0); localtime_s(&now, &t);

    int exists = 0;
    sql << "SELECT 1 FROM public.employee WHERE employee_id = :id", soci::use(emp_id), soci::into(exists);
    if (!exists) throw std::runtime_error("ERROR: Employee ID=" + std::to_string(emp_id) + " not found.");
    exists = 0;
    sql << "SELECT 1 FROM public.category WHERE category_id = :id", soci::use(cat_id), soci::into(exists);
    if (!exists) throw std::runtime_error("ERROR: Category ID=" + std::to_string(cat_id) + " not found.");

    sql << "INSERT INTO public.document (document_id, employee_id, category_id, title, description, date_created, date_updated, version) "
        "VALUES (:id, :eid, :cid, :ti, :d, :cr, :up, :v)",
        soci::use(new_id), soci::use(emp_id), soci::use(cat_id), soci::use(title), soci::use(desc),
        soci::use(now), soci::use(now), soci::use(version);
}

void Model::addFile(int doc_id, const string& name, const string& format, int size) {
    checkDbConnection();
    long long new_id = getLastId("file", "file_id") + 1;
    std::tm now = {}; time_t t = time(0); localtime_s(&now, &t);

    int exists = 0;
    sql << "SELECT 1 FROM public.document WHERE document_id = :id", soci::use(doc_id), soci::into(exists);
    if (!exists) throw std::runtime_error("ERROR: Document ID=" + std::to_string(doc_id) + " not found.");

    sql << "INSERT INTO public.file (file_id, document_id, file_name, format, size, upload_date) "
        "VALUES (:id, :did, :n, :f, :s, :up)",
        soci::use(new_id), soci::use(doc_id), soci::use(name), soci::use(format), soci::use(size), soci::use(now);
}

void Model::addAccess(int emp_id, int doc_id, const string& level, const string& expiry_str) {
    checkDbConnection();
    long long new_id = getLastId("access", "access_id") + 1;
    std::tm now = {}; time_t t = time(0); localtime_s(&now, &t);
    std::tm expiry_tm = stringToTm(expiry_str);

    int emp_exists = 0, doc_exists = 0;
    sql << "SELECT 1 FROM public.employee WHERE employee_id = :id", soci::use(emp_id), soci::into(emp_exists);
    if (!emp_exists) throw std::runtime_error("ERROR: Employee ID=" + std::to_string(emp_id) + " not found.");
    sql << "SELECT 1 FROM public.document WHERE document_id = :id", soci::use(doc_id), soci::into(doc_exists);
    if (!doc_exists) throw std::runtime_error("ERROR: Document ID=" + std::to_string(doc_id) + " not found.");

    sql << "INSERT INTO public.access (access_id, employee_id, document_id, access_level, date_granted, expiry_date) "
        "VALUES (:id, :eid, :did, :l, :gr, :ex)",
        soci::use(new_id), soci::use(emp_id), soci::use(doc_id), soci::use(level), soci::use(now), soci::use(expiry_tm);
}

void Model::updateEmployee(int id, const string& first, const string& last, const string& email, const string& dep) {
    checkDbConnection();
    sql << "UPDATE public.employee SET first_name = :f, last_name = :l, email = :e, department = :d WHERE employee_id = :id",
        soci::use(first), soci::use(last), soci::use(email), soci::use(dep), soci::use(id);
}

void Model::updateCategory(int id, const string& name, const string& desc, const string& type) {
    checkDbConnection();
    sql << "UPDATE public.category SET category_name = :n, description = :d, type = :t WHERE category_id = :id",
        soci::use(name), soci::use(desc), soci::use(type), soci::use(id);
}

void Model::updateDocument(int id, const string& title, const string& desc, int version) {
    checkDbConnection();
    std::tm now = {}; time_t t = time(0); localtime_s(&now, &t);

    sql << "UPDATE public.document SET title = :ti, description = :d, version = :v, date_updated = :up WHERE document_id = :id",
        soci::use(title), soci::use(desc), soci::use(version), soci::use(now), soci::use(id);
}

void Model::updateFile(int id, const string& name, const string& format, int size) {
    checkDbConnection();
    sql << "UPDATE public.file SET file_name = :n, format = :f, size = :s WHERE file_id = :id",
        soci::use(name), soci::use(format), soci::use(size), soci::use(id);
}

void Model::updateAccess(int id, const string& level, const string& expiry_str) {
    checkDbConnection();
    std::tm expiry_tm = stringToTm(expiry_str);

    sql << "UPDATE public.access SET access_level = :l, expiry_date = :ex WHERE access_id = :id",
        soci::use(level), soci::use(expiry_tm), soci::use(id);
}

void Model::deleteData(const string& table_name, const string& id_col, int id) {
    checkDbConnection();
    try {
        sql << "DELETE FROM public.\"" + table_name + "\" WHERE \"" + id_col + "\" = :id", soci::use(id);
    }
    catch (const std::exception& e) {
        string msg = e.what();
        if (msg.find("violates foreign key constraint") != string::npos) {
            throw std::runtime_error("ERROR: Cannot delete record ID=" + std::to_string(id) + " from '" + table_name + "', it's referenced by other tables.");
        }
        else { throw std::runtime_error("ERROR during deletion: " + msg); }
    }
}

void Model::generateEmployees(int count) {
    checkDbConnection();
    sql << R"( INSERT INTO public.employee (employee_id, first_name, last_name, email, department) SELECT n + (SELECT COALESCE(MAX(employee_id), 0) FROM public.employee), 'FirstName_' || (RANDOM() * 1000)::int, 'LastName_' || (RANDOM() * 1000)::int, 'user' || (n + (SELECT COALESCE(MAX(employee_id), 0) FROM public.employee)) || '@example.com', (ARRAY['Development', 'Analytics', 'HR', 'Support'])[ (RANDOM() * 3 + 1)::int ] FROM generate_series(1, :count) AS s(n) ON CONFLICT (employee_id) DO NOTHING )", soci::use(count);
}
void Model::generateCategories(int count) {
    checkDbConnection();
    sql << R"( INSERT INTO public.category (category_id, category_name, description, type) SELECT n + (SELECT COALESCE(MAX(category_id), 0) FROM public.category), 'Category ' || (RANDOM() * 1000)::int, 'Description ' || n, (ARRAY['Internal', 'Public', 'Confidential'])[ (RANDOM() * 2 + 1)::int ] FROM generate_series(1, :count) AS s(n) ON CONFLICT (category_id) DO NOTHING )", soci::use(count);
}
void Model::generateDocuments(int count) {
    checkDbConnection();
    long long emp_c = 0, cat_c = 0; sql << "SELECT COUNT(*) FROM public.employee", soci::into(emp_c); sql << "SELECT COUNT(*) FROM public.category", soci::into(cat_c);
    if (emp_c == 0 || cat_c == 0) throw std::runtime_error("ERROR: Generate employees and categories first.");
    sql << R"( INSERT INTO public.document (document_id, employee_id, category_id, title, description, date_created, date_updated, version) SELECT n + (SELECT COALESCE(MAX(document_id), 0) FROM public.document), (SELECT employee_id FROM public.employee ORDER BY RANDOM() LIMIT 1), (SELECT category_id FROM public.category ORDER BY RANDOM() LIMIT 1), 'Document Title ' || n, 'Generated description', NOW() - (RANDOM() * '365 days'::interval), NOW() - (RANDOM() * '30 days'::interval), (RANDOM() * 5 + 1)::int FROM generate_series(1, :count) AS s(n) ON CONFLICT (document_id) DO NOTHING )", soci::use(count);
}
void Model::generateFiles(int count) {
    checkDbConnection(); long long doc_c = 0; sql << "SELECT COUNT(*) FROM public.document", soci::into(doc_c);
    if (doc_c == 0) throw std::runtime_error("ERROR: Generate documents first.");
    sql << R"( INSERT INTO public.file (file_id, document_id, file_name, format, size, upload_date) SELECT n + (SELECT COALESCE(MAX(file_id), 0) FROM public.file), (SELECT document_id FROM public.document ORDER BY RANDOM() LIMIT 1), 'file_' || n || (ARRAY['.pdf', '.docx', '.txt', '.zip'])[ (RANDOM() * 3 + 1)::int ], (ARRAY['PDF', 'DOCX', 'TXT', 'ZIP'])[ (RANDOM() * 3 + 1)::int ], (RANDOM() * 10000 + 100)::int, NOW() - (RANDOM() * '90 days'::interval) FROM generate_series(1, :count) AS s(n) ON CONFLICT (file_id) DO NOTHING )", soci::use(count);
}
void Model::generateAccess(int count) {
    checkDbConnection(); long long emp_c = 0, doc_c = 0; sql << "SELECT COUNT(*) FROM public.employee", soci::into(emp_c); sql << "SELECT COUNT(*) FROM public.document", soci::into(doc_c);
    if (emp_c == 0 || doc_c == 0) throw std::runtime_error("ERROR: Generate employees and documents first.");
    sql << R"( INSERT INTO public.access (access_id, employee_id, document_id, access_level, date_granted, expiry_date) SELECT n + (SELECT COALESCE(MAX(access_id), 0) FROM public.access), (SELECT employee_id FROM public.employee ORDER BY RANDOM() LIMIT 1), (SELECT document_id FROM public.document ORDER BY RANDOM() LIMIT 1), (ARRAY['read', 'edit', 'delete'])[ (RANDOM() * 2 + 1)::int ], NOW() - (RANDOM() * '10 days'::interval), NOW() + (RANDOM() * '365 days'::interval) FROM generate_series(1, :count) AS s(n) ON CONFLICT (access_id) DO NOTHING )", soci::use(count);
}

double Model::searchDocsByDeptAndCat(const string& department, const string& category_type) {
    checkDbConnection();
    string dept_pattern = "%" + department + "%"; string cat_pattern = "%" + category_type + "%";
    int count = 0;
    clock_t start = clock();

    soci::rowset<soci::row> rs = (sql.prepare <<
        R"( SELECT e.first_name, e.last_name, d.title, c.category_name FROM public.employee e
            JOIN public.document d ON e.employee_id = d.employee_id JOIN public.category c ON d.category_id = c.category_id
            WHERE e.department LIKE :dept AND c.type LIKE :cat_type )",
        soci::use(dept_pattern, "dept"), soci::use(cat_pattern, "cat_type"));

    std::cout << "\n--- Search Results (Docs by Dept & Category Type) ---\n";
    std::cout << std::left
        << std::setw(30) << "Author" << " | "
        << std::setw(50) << "Document Title" << " | "
        << std::setw(25) << "Category" << "\n";
    std::cout << string(111, '-') << "\n";

    for (const auto& r : rs) {
        std::cout << std::left
            << std::setw(30) << truncate(r.get<string>(0) + " " + r.get<string>(1), 30) << " | "
            << std::setw(40) << truncate(r.get<string>(2), 50) << " | "
            << std::setw(25) << truncate(r.get<string>(3), 25) << "\n";
        count++;
    }

    clock_t end = clock();
    if (count == 0) std::cout << "No results found.\n";
    double ms_double = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    return ms_double;
}

double Model::searchFilesByAccessAndSize(const string& access_level, int min_size, int max_size) {
    checkDbConnection();
    string level_pattern = "%" + access_level + "%";
    int count = 0;
    clock_t start = clock();

    soci::rowset<soci::row> rs = (sql.prepare <<
        R"( SELECT f.file_name, f.format, f.size, d.title, a.access_level FROM public.file f
            JOIN public.document d ON f.document_id = d.document_id JOIN public.access a ON d.document_id = a.document_id
            WHERE a.access_level LIKE :level AND f.size BETWEEN :min_s AND :max_s ORDER BY f.size DESC )",
        soci::use(level_pattern, "level"), soci::use(min_size, "min_s"), soci::use(max_size, "max_s"));

    std::cout << "\n--- Search Results (Files by Access & Size) ---\n";
    std::cout << std::left
        << std::setw(30) << "File" << " | "
        << std::setw(50) << "Document" << " | "
        << std::setw(10) << "Size(KB)" << " | "
        << std::setw(15) << "Access" << "\n";
    std::cout << string(114, '-') << "\n";

    for (const auto& r : rs) {
        std::cout << std::left
            << std::setw(30) << truncate(r.get<string>(0), 30) << " | "
            << std::setw(30) << truncate(r.get<string>(3), 50) << " | "
            << std::setw(10) << r.get<int>(2) << " | "
            << std::setw(15) << truncate(r.get<string>(4), 15) << "\n";
        count++;
    }

    clock_t end = clock();
    if (count == 0) std::cout << "No results found.\n";
    double ms_double = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    return ms_double;
}

double Model::searchDocCountByEmployeeAndDate(const string& date_start_str, const string& date_end_str) {
    checkDbConnection();
    std::tm date_start = stringToTm(date_start_str);
    std::tm date_end = stringToTm(date_end_str);
    int count = 0;
    clock_t start = clock();

    soci::rowset<soci::row> rs = (sql.prepare <<
        R"( SELECT e.first_name, e.last_name, COUNT(d.document_id) as doc_count FROM public.employee e
            JOIN public.document d ON e.employee_id = d.employee_id WHERE d.date_created BETWEEN :start AND :end
            GROUP BY e.employee_id, e.first_name, e.last_name ORDER BY doc_count DESC )",
        soci::use(date_start, "start"), soci::use(date_end, "end"));

    std::cout << "\n--- Search Results (Doc Count by Author & Date) ---\n";
    std::cout << std::left
        << std::setw(40) << "Author" << " | "
        << std::setw(15) << "Document Count" << "\n";
    std::cout << string(50, '-') << "\n";

    for (const auto& r : rs) {
        std::cout << std::left
            << std::setw(40) << truncate(r.get<string>(0) + " " + r.get<string>(1), 30) << " | "
            << std::setw(15) << r.get<long long>(2) << "\n";
        count++;
    }

    clock_t end = clock();
    if (count == 0) std::cout << "No results found.\n";
    double ms_double = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    return ms_double;
}