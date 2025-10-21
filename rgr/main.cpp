#include "Controller.h" 
#include "View.h"      

int main() {
    View mainView;
    try {
        Controller app("dbname=postgres user=postgres password=1234567890 host=localhost port=5432");
        app.main_run();
    }
    catch (const std::exception& e) {
        std::cerr << "Application failed to start or encountered a critical error: " << e.what() << std::endl;
        mainView.showMessage("Exiting due to critical error...");
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown critical error occurred." << std::endl;
        mainView.showMessage("Exiting due to unknown critical error...");
        return 2;
    }

    mainView.showMessage("Exiting...");
    return 0;
}