const uploadMessage = document.getElementById('upload-message');

// Server status check
function checkUploadServer() {
    fetch('http://localhost:8084/api/health')
        .then(res => {
            if (!res.ok) throw new Error('Server not responding');
            // Show forms and hide error
            document.querySelector('.upload-methods').style.display = 'grid';
            uploadMessage.classList.add('hidden');
        })
        .catch(() => showUnavailableMessage());
}
// Show service unavailable message
function showUnavailableMessage() {
    // Hide upload forms
    document.querySelector('.upload-methods').style.display = 'none';
    
    // Show error message
    uploadMessage.textContent = 'Grade upload service is currently unavailable. Please try again later.';
    uploadMessage.className = 'error';
    uploadMessage.classList.remove('hidden');
}

// Handle API responses
function handleResponse(response, successMessage) {
    uploadMessage.classList.remove('hidden');
    
    if (response.ok) {
        uploadMessage.textContent = successMessage;
        uploadMessage.className = 'success';
        return response.json();
    } else {
        return response.json().then(err => {
            throw new Error(err.error || 'Request failed');
        });
    }
}

// Single Grade Upload
document.getElementById('manual-upload').addEventListener('submit', async (e) => {
    e.preventDefault();
    uploadMessage.classList.add('hidden');

    try {
        const response = await fetch('http://localhost:8084/api/upload/single', {
            method: 'POST',
            headers: {
                'Authorization': `Bearer ${localStorage.getItem('authToken')}`,
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                student: document.getElementById('student-select').value,
                course: document.getElementById('course-select').value,
                grade: document.getElementById('grade').value
            })
        });

        await handleResponse(response, 'Grade submitted successfully!');
        e.target.reset(); // Clear form on success
        
    } catch (error) {
        uploadMessage.textContent = error.message;
        uploadMessage.className = 'error';
    }
});

// CSV Upload
document.getElementById('csv-upload').addEventListener('submit', async (e) => {
    e.preventDefault();
    uploadMessage.classList.add('hidden');

    const fileInput = document.getElementById('csv-file');
    if (!fileInput.files.length) {
        uploadMessage.textContent = 'Please select a CSV file';
        uploadMessage.className = 'error';
        uploadMessage.classList.remove('hidden');
        return;
    }

    try {
        const formData = new FormData();
        formData.append('csv', fileInput.files[0]);

        const response = await fetch('http://localhost:8084/api/upload/csv', {
            method: 'POST',
            headers: {
                'Authorization': `Bearer ${localStorage.getItem('authToken')}`
            },
            body: formData
        });

        const data = await handleResponse(response, 'CSV processed successfully!');
        if (data.invalidEntries && data.invalidEntries.length > 0) {
            uploadMessage.textContent += ` (${data.invalidEntries.length} invalid entries)`;
        }
        fileInput.value = ''; // Clear file input
        
    } catch (error) {
        uploadMessage.textContent = error.message;
        uploadMessage.className = 'error';
    }
});

// Initial health check
window.addEventListener('load', () => {
    checkUploadServer();
    // Refresh server status every 30 seconds
    setInterval(checkUploadServer, 30000);
});