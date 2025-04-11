#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include "../backend/include/jwt-cpp/jwt.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <sstream> 

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

// mock database for grades
std::vector<Grade> getStudentGrades(const std::string& studentId) {
    if (studentId == "student1@dlsu.edu.ph") {
        return {
            Grade("CSSECDV", "Secure Web Development", "3.0", "1st", "2024-2025"),
            Grade("LASARE2", "Lasallian Reflection 2", "3.5", "1st", "2024-2025"),
            Grade("STDISCM", "Distributed Computing", "4.0", "1st", "2024-2025"),
        };
    } else if (studentId == "student2@dlsu.edu.ph") {
        return {
            Grade("STDISCM", "Distributed Computing", "4.0", "1st", "2024-2025"),
            Grade("MOBDEVE", "Mobile Development", "4.0", "1st", "2024-2025"),
            Grade("CCINOV8", "Innovation and Technology Management", "3.5", "1st", "2024-2025"),
            Grade("GEUSELF", "Understanding the Self", "2.5", "1st", "2024-2025"),
            Grade("STMETHD", "ST Research Methods", "4.0", "1st", "2024-2025"),
        };
    } else if (studentId == "student3@dlsu.edu.ph") {
        return {
            Grade("STINTSY", "Advanced Intelligent Systems", "3.5", "1st", "2024-2025"),
            Grade("STMETHD", "ST Research Methods", "3.5", "1st", "2024-2025"),
            Grade("SAS3000", "Student Affairs Services 3000", "4.0", "1st", "2024-2025"),
            Grade("STHCUIX", "Human Computer Interactions", "3.5", "1st", "2024-2025"),
            Grade("GEARTAP", "Art Appreciation", "3.0", "1st", "2024-2025"),
            Grade("STDISCM", "Distributed Computing", "4.0", "1st", "2024-2025")

        };
    }
    return {};
}

std::string readSecretKeyFromConfig() {
    std::ifstream configFile("../config.txt");
    std::string line;
    while (std::getline(configFile, line)) {
        if (line.find("JWT_SECRET_KEY=") != std::string::npos) {
            return line.substr(line.find('=') + 1);
        }
    }
    throw std::runtime_error("JWT_SECRET_KEY not found in config.txt");
}

int main() {
    httplib::Server svr;

    // API Endpoint: GET /api/grades (requires Authorization)
    svr.Get("/api/grades", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string authorizationHeader = req.get_header_value("Authorization");
            if (authorizationHeader.empty() || authorizationHeader.find("Bearer ") != 0) {
                res.status = 401;
                res.set_content("{\"error\": \"Unauthorized\"}", "application/json");
                return;
            }

            std::string secretKey;
            try {
                secretKey = readSecretKeyFromConfig();
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content("{\"error\": \"Failed to read JWT secret from config\"}", "application/json");
                return;
            }

        // Verify and decode token
        std::string token = authorizationHeader.substr(7);
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{secretKey});
        verifier.verify(decoded);

        
        // Extract user ID from token
        std::string studentId = decoded.get_payload_claim("sub").as_string();
        std::cout << "Extracted studentId from token: " << studentId << std::endl;

        // Get grades for this specific student
        std::vector<Grade> grades = getStudentGrades(studentId);

            json json_response = json::array();
            for (const auto& grade : grades) {
                json_response.push_back(grade.toJson());
            }

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Content-Type", "application/json");
            res.set_content(json_response.dump(), "application/json");
            std::cout << "Successfully sent grades for student " << studentId << "." << std::endl;

        }  catch (const jwt::error::signature_verification_exception& e) {
            res.status = 401;
            res.set_content("{\"error\": \"Invalid token\"}", "application/json");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("{\"error\": \"" + std::string(e.what()) + "\"}", "application/json");
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