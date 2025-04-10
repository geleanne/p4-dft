#include "../lib/httplib.h"
#include "../lib/json.hpp"
#include "../backend/vcpkg/installed/x64-windows/include/jwt-cpp/jwt.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>

using json = nlohmann::json;

std::string generateJWT(const std::string& userId, const std::string& userRole, const std::string& secretKey) {
    auto now = std::chrono::system_clock::now();
    auto exp = now + std::chrono::minutes(60);

    auto token = jwt::create()
        .set_issuer("your_issuer")
        .set_type("JWS")
        .set_payload_claim("user_id", jwt::claim(userId))
        .set_payload_claim("role", jwt::claim(userRole))
        .set_issued_at(now)
        .set_expires_at(exp)
        .sign(jwt::algorithm::hs256{secretKey});

    return token;
}

int main() {
    httplib::Server svr;

    std::map<std::string, std::string> hardcodedAccounts = {
        {"student1@dlsu.edu.ph", "pass123"},
        {"faculty1@dlsu.edu.ph", "secure456"},
        {"admin@dlsu.edu.ph", "admin123"}
    };

    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            if (req.body.empty()) {
                res.status = 400;
                res.set_content("{\"error\": \"Request body is empty.\"}", "application/json");
                return;
            }

            try {
                json body = json::parse(req.body);
                std::string username = body["username"];
                std::string password = body["password"];

                std::cout << "Username received: " << username << std::endl;
                std::cout << "Password received: " << password << std::endl;

                if (username.find("@dlsu.edu.ph") == std::string::npos) {
                    res.status = 400;
                    res.set_content("{\"error\": \"Invalid email domain. Must end with @dlsu.edu.ph\"}", "application/json");
                    return;
                }

                if (hardcodedAccounts.find(username) != hardcodedAccounts.end() && hardcodedAccounts[username] == password) {
                    std::string role = "student";
                    if (username == "faculty1@dlsu.edu.ph") role = "faculty";
                    else if (username == "admin@dlsu.edu.ph") role = "admin";
                    std::cout << "Login Successful" << std::endl;

                    
                    const char* secretKeyPtr = std::getenv("JWT_SECRET_KEY");
                    if (!secretKeyPtr) {
                        res.status = 500;
                        res.set_content("{\"error\": \"JWT_SECRET_KEY not set!\"}", "application/json");
                        return;
                    }
                    std::string secretKey(secretKeyPtr);

                    std::string token = generateJWT(username, role, secretKey);

                    json response = {
                        {"token", token},
                        {"message", "Login successful"}
                    };

                    res.set_header("Content-Type", "application/json");
                    res.set_content(response.dump(), "application/json");
                } else {
                    res.status = 401;
                    res.set_content("{\"error\": \"Invalid username or password\"}", "application/json");
                }
            } catch (const json::parse_error& e) {
                res.status = 400;
                res.set_content("{\"error\": \"Invalid JSON in request body.\"}", "application/json");
                return;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error processing login: " << e.what() << std::endl;
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