# P4 - Distributed Fault Tolerance

For this exercise, we are tasked to create an **online enrollment system** with different services distributed across multiple nodes.

## Build/Compilation Steps (MinGW)
### Frontend 
1. Open `login.html` using Live Server (VSCode)
2. On the login page, click **View Course Offerings** at the bottom.

### Backend (View available courses)
1. Navigate to the backend folder by typing `cd backend` in the terminal.
2. Compile the backend using the following command:\
`g++ src/main.cpp -o course_server.exe -I./include -I./lib -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17`
3. Run the backend: `course_server.exe` in the terminal.
4. **Available courses** should now be viewable.

### Backend (Students to view previous grades)
1. Navigate to the backend folder by typing `cd grades_backend` in the terminal.
2. Compile the backend using the following command:\
`g++ grades_server.cpp -o grades_server.exe -I./include -I./lib -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17`
3. Run the backend: `grades_server.exe` in the terminal.
4. **Students' previous grades** should now be viewable.
