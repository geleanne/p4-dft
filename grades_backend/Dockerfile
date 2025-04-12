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

# Copy grades_backend source
COPY grades_backend /app/grades_backend

# Set working directory to grades_backend
WORKDIR /app/grades_backend

# Compile the code
# -I/app/lib for httplib.h and json.hpp
# -I/app/backend/include for jwt.h
# -lssl -lcrypto for OpenSSL
# -pthread for threading
RUN g++ -o grades_server grades_server.cpp -I/app/lib -I/app/backend/include -lssl -lcrypto -pthread -std=c++17

# Expose port 8085 (assumed; original code uses 8083, conflicting with Login)
EXPOSE 8085

# Run the server
CMD ["./grades_server"]