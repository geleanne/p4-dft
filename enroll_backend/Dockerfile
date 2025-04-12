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

# Copy enroll_backend source
COPY enroll_backend /app/enroll_backend

# Set working directory to enroll_backend
WORKDIR /app/enroll_backend

# Compile the code
# -I/app/lib for httplib.h and json.hpp
# -I/app/backend/include for jwt.h
# -lssl -lcrypto for OpenSSL
# -pthread for threading
RUN g++ -o enroll_server enroll_server.cpp -I/app/lib -I/app/backend/include -lssl -lcrypto -pthread -std=c++17

# Expose port 8082 as specified in the code
EXPOSE 8082

# Run the server
CMD ["./enroll_server"]