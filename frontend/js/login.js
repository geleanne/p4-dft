document.addEventListener('DOMContentLoaded', function() {
    const loginForm = document.getElementById('login-form');
    const loginMessage = document.getElementById('login-message');

    loginForm.addEventListener('submit', async function(event) {
        event.preventDefault();
        const username = document.getElementById('username').value;
        const password = document.getElementById('password').value;

        try {
            const response = await fetch('/api/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ username, password })
            });

            const data = await response.json();

            if (response.ok && data.token) {
                localStorage.setItem('authToken', data.token);
                // redirect to the profile page
                window.location.href = 'profile.html';
            } else {
                loginMessage.textContent = data.message || 'Login failed. Please check your credentials.';
                loginMessage.className = 'error-message';
            }

        } catch (error) {
            console.error('Login error:', error);
            loginMessage.textContent = 'Network error. Please try again later.';
            loginMessage.className = 'error-message';
        }
    });
});