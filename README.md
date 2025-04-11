# P4 - Distributed Fault Tolerance

For this exercise, we are tasked to create an **online enrollment system** with different services distributed across multiple nodes.

## Build/Compilation Steps (MinGW)
### Frontend 
1. Open `login.html` using Live Server (VSCode)
2. On the login page, click **View Course Offerings** at the bottom.

### Backend (Login)
1. Navigate to the backend folder by typing `cd login_backend` in the terminal.
2. Compile the backend using the following command:\
`g++ login_server.cpp -o login_server.exe -I../../lib -I../backend/vcpkg/installed/x64-windows/include -L../backend/vcpkg/installed/x64-windows/lib -lssl -lcrypto -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17`
3. Run the backend: `login_server.exe` in the terminal.
4. **Login form** should now be viewable

### Backend (View available courses)
1. Navigate to the backend folder by typing `cd course_backend` in the terminal.
2. Compile the backend using the following command:\
`g++ course_server.cpp -o course_server.exe -I./include -I./lib -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17`
3. Run the backend: `course_server.exe` in the terminal.
4. **Available courses** should now be viewable.

### Backend (Students to view previous grades)
1. Navigate to the backend folder by typing `cd grades_backend` in the terminal.
2. Compile the backend using the following command:\
`g++ grades_server.cpp -o grades_server.exe -I./include -I./lib -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17`
3. Run the backend: `grades_server.exe` in the terminal.
4. **Students' previous grades** should now be viewable.