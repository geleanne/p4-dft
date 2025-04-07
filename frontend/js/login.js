document.addEventListener('DOMContentLoaded', function() {
    const loginForm = document.getElementById('login-form');
    const loginMessage = document.getElementById('login-message');

    // Hardcoded accounts (username: password)
    const hardcodedAccounts = {
        'student1': 'pass123',
        'faculty1': 'secure456',
        'admin': 'admin123'
    };

    loginForm.addEventListener('submit', function(event) {
        event.preventDefault();
        const username = document.getElementById('username').value;
        const password = document.getElementById('password').value;

        if (hardcodedAccounts.hasOwnProperty(username) && hardcodedAccounts[username] === password) {
            // Successful login
            loginMessage.textContent = ''; // Clear any previous error messages
            loginMessage.className = '';
            // Simulate setting a token (optional, for future enhancements)
            localStorage.setItem('authToken', 'hardcoded_token_' + username);
            window.location.href = 'profile.html'; // Redirect to profile page
        } else {
            // Failed login
            loginMessage.textContent = 'Invalid username or password.';
            loginMessage.className = 'error-message';
        }
    });
});