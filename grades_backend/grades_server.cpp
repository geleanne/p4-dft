#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include <vector>
#include <string>
#include <iostream>

// nlohmann/json library namespace
using json = nlohmann::json;

struct Grade {
    std::string courseCode;
    std::string courseTitle;
    std::string finalGrade;
    std::string semester;
    std::string academicYear;

    Grade(std::string courseCode, std::string courseTitle, std::string finalGrade, std::string semester, std::string academicYear)
        : courseCode(courseCode), courseTitle(courseTitle), finalGrade(finalGrade), semester(semester), academicYear(academicYear) {}

    // convert Grade struct to JSON object
    json toJson() const {
        return json{
            {"courseCode", courseCode},
            {"courseTitle", courseTitle},
            {"finalGrade", finalGrade},
            {"semester", semester},
            {"academicYear", academicYear}
        };
    }
};

// mock database for grades (replace with actual data source)
std::vector<Grade> getStudentGrades(const std::string& studentId) {
    // in a real system, you would query a database based on the studentId
    if (studentId == "123") {
        return {
            Grade("CSSECDV", "Secure Web Development", "3.0", "1st", "2024-2025"),
            Grade("STDISCM", "Distributed Computing", "4.0", "1st", "2024-2025"),
            Grade("GEUSELF", "Understanding the Self", "2.5", "1st", "2024-2025")
        };
    } else if (studentId == "456") {
        return {
            Grade("STINTSY", "Advanced Intelligent Systems", "3.5", "1st", "2024-2025")
        };
    }
    return {};
}

int main() {
    httplib::Server svr;

    // API Endpoint: GET /api/grades (requires Authorization)
    svr.Get("/api/grades", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Received request for /api/grades" << std::endl;
        try {
            // in a real system, you would extract the student ID from the JWT token
            // for this mock, we'll just use a hardcoded student ID
            std::string studentId = "123"; // replace with actual student ID retrieval

            // Authorization header (Bearer token)
            std::string authorizationHeader = req.get_header_value("Authorization");
            if (authorizationHeader.empty() || authorizationHeader.find("Bearer ") != 0) {
                res.status = 401;
                res.set_content("{\"error\": \"Unauthorized: Missing or invalid Bearer token.\"}", "application/json");
                return;
            }

            // in a real system, we would verify the JWT token here
            std::string token = authorizationHeader.substr(7); // extract the token

            // for this mock, we'll just assume the token is valid and extract the student ID

            std::vector<Grade> grades = getStudentGrades(studentId);

            json json_response = json::array();
            for (const auto& grade : grades) {
                json_response.push_back(grade.toJson());
            }

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Content-Type", "application/json");
            res.set_content(json_response.dump(), "application/json");
            std::cout << "Successfully sent grades for student " << studentId << "." << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error processing /api/grades: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"Internal server error processing grade data.\"}", "application/json");
        } catch (...) {
            std::cerr << "Unknown error processing /api/grades." << std::endl;
            res.status = 500;
            res.set_content("{\"error\": \"An unknown internal server error occurred.\"}", "application/json");
        }
    });

    svr.Options("/api/grades", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.status = 204;
    });

    // --- start Grades Server ---
    std::string host = "0.0.0.0"; // listen on all available network interfaces
    int port = 8081;               // use a different port for the grades server

    std::cout << "Grades backend server starting on " << host << ":" << port << std::endl;
    if (!svr.listen(host.c_str(), port)) {
        std::cerr << "Failed to start grades server on port " << port << std::endl;
        return 1;
    }

    return 0;
}