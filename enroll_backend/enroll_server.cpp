#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include "../backend/include/jwt-cpp/jwt.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

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

    // Enrollment endpoint
    svr.Post("/api/enroll", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string authorizationHeader = req.get_header_value("Authorization");
            if (authorizationHeader.empty() || authorizationHeader.find("Bearer ") != 0) {
                res.status = 401;
                res.set_content("{\"error\": \"Unauthorized\"}", "application/json");
                return;
            }

            std::string secretKey = readSecretKeyFromConfig();
            std::string token = authorizationHeader.substr(7);
            
            // Verify JWT
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{secretKey});
            verifier.verify(decoded);

            // Check user role
            std::string role = decoded.get_payload_claim("role").as_string();
            if (role != "student") {
                res.status = 403;
                res.set_content("{\"error\": \"Forbidden - Students only\"}", "application/json");
                return;
            }

            // Process enrollment
            json body = json::parse(req.body);
            std::string courseId = body["courseId"];
            
            // Add your actual enrollment logic here
            json response = {
                {"success", true},
                {"message", "Successfully enrolled in course " + courseId}
            };

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(response.dump(), "application/json");

        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    // CORS preflight
    svr.Options("/api/enroll", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.status = 204;
    });

    // Health check
    svr.Get("/api/health", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("OK", "text/plain");
    });

    const char* port_str = std::getenv("PORT");
    if (!port_str) {
        std::cerr << "PORT environment variable not set." << std::endl;
        return 1;
    }
    int port;
    try {
        port = std::stoi(port_str);
    } catch (const std::exception& e) {
        std::cerr << "Invalid PORT value: " << port_str << std::endl;
        return 1;
    }

    std::cout << "Enrollment server starting on 0.0.0.0:" << port << "\n";
    svr.listen("0.0.0.0", port);
    return 0;
}