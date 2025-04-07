// #define CPPHTTPLIB_OPENSSL_SUPPORT // requires OpenSSL libraries

#include "../../lib/httplib.h"
#include "../../lib/json.hpp"
#include <vector>
#include <string>
#include <iostream>

// nlohmann/json library namespace
using json = nlohmann::json;

struct Course {
    std::string id;
    std::string title;
    int capacity;
    int enrolled;

    Course(std::string id, std::string title, int capacity, int enrolled)
        : id(id), title(title), capacity(capacity), enrolled(enrolled) {}

    // convert Course struct to JSON object
    json toJson() const {
        return json{
            {"id", id},
            {"title", title},
            {"capacity", capacity},
            {"enrolled", enrolled},
            {"isOpen", enrolled < capacity} // a flag if the course is open
        };
    }
};

// mock database
std::vector<Course> getAvailableCourses() {
    return {
        Course("STMETHD", "ST Reseach Methods", 30, 21),
        Course("MOBDEVE", "Mobile Development", 25, 25), // example of a closed course
        Course("GEUSELF", "Understanding the Self", 40, 15),
        Course("CSOPESY", "Introduction to Operating Systems", 42, 30),
        Course("STSWENG", "Advanced Software Engineering", 35, 19),
        Course("PRCCSST", "Practicum for ST Students", 40, 9),
    };
}


int main() {
    httplib::Server svr;

    // API Endpoint: GET /api/courses
    svr.Get("/api/courses", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Received request for /api/courses" << std::endl;
        try {
            // get course data (from our mock function)
            std::vector<Course> courses = getAvailableCourses();

            // vector of Course structs to a JSON array
            json json_response = json::array();
            for (const auto& course : courses) {
                json_response.push_back(course.toJson());
            }

            // set response headers
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Content-Type", "application/json");

            // set response body
            res.set_content(json_response.dump(), "application/json");
            std::cout << "Successfully sent course list." << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error processing /api/courses: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Internal server error processing course data.\"}", "application/json");
        } catch (...) {
            std::cerr << "Unknown error processing /api/courses." << std::endl;
            res.status = 500;
             res.set_content("{\"error\": \"An unknown internal server error occurred.\"}", "application/json");
        }
    });

     svr.Options("/api/.*", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization"); 
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"); 
        res.status = 204; 
    });

    // --- start Server ---
    std::string host = "0.0.0.0"; // listen on all available network interfaces
    int port = 8080;        // port for the backend API

    std::cout << "Backend server starting on " << host << ":" << port << std::endl;
    if (!svr.listen(host.c_str(), port)) {
         std::cerr << "Failed to start server on port " << port << std::endl;
         return 1; 
    }

    return 0;
}