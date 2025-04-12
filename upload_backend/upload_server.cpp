#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include "../backend/include/jwt-cpp/jwt.h"
#include <iostream>
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

    // Middleware for JWT Verification
    svr.Post("/api/upload/.*", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string token = req.get_header_value("Authorization").substr(7);
            std::string secretKey = readSecretKeyFromConfig();
            
            auto decoded = jwt::decode(token);
            jwt::verify().allow_algorithm(jwt::algorithm::hs256{secretKey}).verify(decoded);
            
            if(decoded.get_payload_claim("role").as_string() != "faculty") {
                res.status = 403;
                res.set_content("{\"error\":\"Forbidden\"}", "application/json");
                return false; // Stop processing
            }
            return true; // Continue processing
        } catch (...) {
            res.status = 401;
            res.set_content("{\"error\":\"Unauthorized\"}", "application/json");
            return false;
        }
    });

    // Single Grade Upload Endpoint
    svr.Post("/api/upload/single", [](const httplib::Request& req, httplib::Response& res) {
        json body = json::parse(req.body);
        // Process single grade entry
        res.set_content("{\"status\":\"success\"}", "application/json");
    });

    // CSV Upload Endpoint
    svr.Post("/api/upload/csv", [](const httplib::Request& req, httplib::Response& res) {
        const auto& file = req.get_file_value("csv");
        std::istringstream ss(file.content);
        std::string line;
        
        while (std::getline(ss, line)) {
            // Process CSV line
        }
        res.set_content("{\"processed\":true}", "application/json");
    });

    svr.Get("/api/health", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("OK", "text/plain");
    });
    
    // Start Server
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

    std::cout << "Upload server starting on 0.0.0.0:" << port << "\n";
    svr.listen("0.0.0.0", port);
    return 0;
}