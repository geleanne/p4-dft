# P4 - Distributed Fault Tolerance

![image](https://github.com/user-attachments/assets/5cda44a6-4812-4cbf-9bdb-5b14396cef56)

For this exercise, we are tasked to create an **online enrollment system** with different services distributed across multiple nodes.

### Authors: STDISCM S12 discm buddies 
- ATIENZA, MARIELLE ANGELENE
- BERNARDO, ERICA MONTEMAYOR
- RIVERA, JOSE CARLOS IGNACIO
- TIGHE, KAITLYN PATRICIA

Included in this repository are:
1. Source code
3. [Presentation Slides](https://docs.google.com/presentation/d/1Hbs5G44UKEXeiogWgGzpouL69MXJfP2WY7fe5CzyAFw/edit?usp=sharing)
4. [P4 Demo Video](https://youtu.be/nWF8DwlFmvY) - timestamps available in the description of the video 


#### Build Commands for Docker
docker build -f login_backend/Dockerfile . -t login_backend\
docker build -f course_backend/Dockerfile . -t course_backend\
docker build -f enroll_backend/Dockerfile . -t enroll_backend\
docker build -f grades_backend/Dockerfile . -t grades_backend\
docker build -f upload_backend/Dockerfile . -t upload_backend\
docker build -f frontend/Dockerfile . -t frontend\
docker-compose build\
docker-compose up

### Access the Frontend in Browser
Open http://localhost in your browser
