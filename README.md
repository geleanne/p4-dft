# P4 - Distributed Fault Tolerance

For this exercise, we are tasked to create an **online enrollment system** with different services distributed across multiple nodes.

#### Build Commands for Dockerfile
docker build -f login_backend/Dockerfile . -t login_backend
docker build -f course_backend/Dockerfile . -t course_backend
docker build -f enroll_backend/Dockerfile . -t enroll_backend
docker build -f grades_backend/Dockerfile . -t grades_backend
docker build -f upload_backend/Dockerfile . -t upload_backend
docker build -f frontend/Dockerfile . -t frontend
docker-compose build
docker-compose up