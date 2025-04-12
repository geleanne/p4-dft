FROM ubuntu:20.04

# Install dependencies: g++ for compilation
RUN apt-get update && apt-get install -y \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy shared files (only lib is needed)
COPY lib /app/lib

# Copy course_backend source
COPY course_backend /app/course_backend

# Set working directory to course_backend
WORKDIR /app/course_backend

# Compile the code
# -I/app/lib for httplib.h and json.hpp
# -pthread for potential threading in httplib
RUN g++ -o course_server course_server.cpp -I/app/lib -pthread -std=c++17

# Expose port 8080 as specified in the code
EXPOSE 8080

# Run the server
CMD ["./course_server"]