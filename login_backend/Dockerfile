FROM ubuntu:20.04

# Install dependencies: g++ for compilation, libssl-dev for OpenSSL
RUN apt-get update && apt-get install -y \
    g++ \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy shared files from the root directory
COPY lib /app/lib
COPY backend/include /app/backend/include
COPY config.txt /app/config.txt

# Copy login_backend source
COPY login_backend /app/login_backend

# Set working directory to login_backend
WORKDIR /app/login_backend

# Compile the code
# -I/app/lib for httplib.h and json.hpp
# -I/app/backend/include for jwt.h
# -lssl -lcrypto for OpenSSL
# -pthread for threading (replaces -lpthread from MinGW)
RUN g++ -o login_server login_server.cpp -I/app/lib -I/app/backend/include -lssl -lcrypto -pthread -std=c++17

# Expose port 8083 as specified in the code
EXPOSE 8083

# Run the server
CMD ["./login_server"]