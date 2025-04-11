#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include "../backend/include/jwt-cpp/jwt.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <fstream> 
#include <sstream>
#include <fstream> 
#include <sstream> 

using json = nlohmann::json;

std::string generateJWT(const std::string& userId, const std::string& userRole, const std::string& secretKey) {
    auto now = std::chrono::system_clock::now();
    auto exp = now + std::chrono::minutes(60);

    auto token = jwt::create()
        .set_issuer("your_issuer")
        .set_type("JWS")
        .set_payload_claim("sub", jwt::claim(userId))
        .set_payload_claim("role", jwt::claim(userRole))
        .set_issued_at(now)
        .set_expires_at(exp)
        .sign(jwt::algorithm::hs256{secretKey});

    return token;
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


    // hardcoded accounts map
    std::map<std::string, std::string> hardcodedAccounts = {
        {"atienza@dlsu.edu.ph", "pass123"},
        {"bernardo@dlsu.edu.ph", "pass456"},
        {"tighe@dlsu.edu.ph", "pass789"},
        {"rivera@dlsu.edu.ph", "pass619"},
        {"faculty_mantua@dlsu.edu.ph", "secure012"},
        {"faculty_delgallego@dlsu.edu.ph", "secure456"},
        {"faculty_delacruz@dlsu.edu.ph", "secure789"}
    };

    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        // Set CORS headers ONCE at the start
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");

            try {
                json body = json::parse(req.body);
                std::string username = body["username"];
                std::string password = body["password"];

                std::cout << "Username received: " << username << std::endl;
                std::cout << "Password received: " << password << std::endl;

        // 1. Check email domain
        if (username.find("@dlsu.edu.ph") == std::string::npos) {
            res.status = 400;
            res.set_content("{\"error\": \"Invalid email domain\"}", "application/json");
            return;
        }

        // 2. Split username into local-part and domain
        size_t at_pos = username.find('@');
        std::string prefix = username.substr(0, at_pos);

        // 3. Determine role
        std::string role = "student"; // Default role
        if (prefix.find("faculty") == 0) { // Starts with "faculty"
            role = "faculty";
        } 
        else if (prefix.find("student") == 0) { // Starts with "student"
            role = "student";
        }

        // 4. Authentication check
        if (hardcodedAccounts.find(username) != hardcodedAccounts.end() && 
            hardcodedAccounts[username] == password) {
            
            std::string secretKey = readSecretKeyFromConfig();
            std::string token = generateJWT(username, role, secretKey);

            json response = {
                {"token", token},
                {"role", role},
                {"message", "Login successful"}
            };

            res.set_content(response.dump(), "application/json");
            std::cout << "Login Successful: " << username << " as " << role << std::endl;
            } else {
                res.status = 401;
                res.set_content("{\"error\": \"Invalid username or password\"}", "application/json");
            }
        } catch (const json::parse_error& e) {
            res.status = 400;
            res.set_content("{\"error\": \"Invalid JSON in request body.\"}", "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content("{\"error\": \"Internal server error\"}", "application/json");
        }
    });
    


    svr.Options("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.status = 204;
    });

    svr.Options("/grades.html", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.status = 204;
    });

    // --- start Server ---
    std::string host = "0.0.0.0";
    int port = 8083;

    std::cout << "Login server starting on " << host << ":" << port << std::endl;
    if (!svr.listen(host.c_str(), port)) {
        std::cerr << "Failed to start server on port " << port << std::endl;
        return 1;
    }

    return 0;
}