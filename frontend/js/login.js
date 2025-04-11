document.addEventListener('DOMContentLoaded', function() {
    const loginForm = document.getElementById('login-form');
    const loginMessage = document.getElementById('login-message');
    const serverDownMessage = document.getElementById('server-down-message');

    loginForm.addEventListener('submit', function(event) {
        event.preventDefault();
        const username = document.getElementById('username').value;
        const password = document.getElementById('password').value;

        fetch('http://localhost:8083/api/login', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ username: username, password: password })
        })
        .then(response => {
            if (response.status === 200) {
              return response.json(); // Proceed for 200 OK
            } else {
              return response.json().then(err => { throw err });
            }
          })
        .then(data => {
            localStorage.setItem('authToken', data.token);
            loginMessage.textContent = 'Logging you in...';
            loginMessage.className = 'success-message';
            setTimeout(() => {
                window.location.href = 'http://127.0.0.1:5500/frontend/grades.html';
            }, 2000); 
        })
        .catch(error => {
            if (error.error) {
                // Check if the error is due to invalid credentials (more robust check)
                if (error.error.includes("Invalid username or password")) {
                    loginMessage.textContent = "Invalid login credentials. Please try again.";
                } else {
                    loginMessage.textContent = error.error;
                }
                loginMessage.className = 'error-message';
                serverDownMessage.style.display = 'none';
            } else if (error.message === "Failed to fetch") {
                serverDownMessage.style.display = 'block';
                loginMessage.textContent = "";
            } else {
                loginMessage.textContent = 'Login failed. Please try again.';
                loginMessage.className = 'error-message';
                serverDownMessage.style.display = 'none';
            }
        });
    });


// Check if the login server is running on page load
    fetch('http://localhost:8083/api/login', { method: 'OPTIONS' })
    .then(() => {
        serverDownMessage.style.display = 'none';
        loginForm.style.display = 'block';
    })
    .catch(() => {
        serverDownMessage.style.display = 'block';
        loginForm.style.display = 'none';
    });
    
});