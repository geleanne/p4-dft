const enrollMessage = document.createElement('div');
enrollMessage.id = 'enroll-message';
document.body.appendChild(enrollMessage);

document.addEventListener('DOMContentLoaded', function() {
    fetchCourses();
});

async function fetchCourses() {
    const courseListDiv = document.getElementById('course-list');
    const apiUrl = 'http://localhost:8080/api/courses';

    try {
        console.log(`Fetching courses from ${apiUrl}...`);
        const response = await fetch(apiUrl);

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const courses = await response.json();
        console.log("Courses received:", courses);

        displayCourses(courses);

    } catch (error) {
        console.error('Error fetching courses:', error);
        courseListDiv.innerHTML = `<p class="error-message">Failed to load courses. The backend service might be down or unreachable. <br><small>(${error.message})</small></p>`;
    }
}

function displayCourses(courses) {
    const courseListDiv = document.getElementById('course-list');
    courseListDiv.innerHTML = '';

    if (!courses || courses.length === 0) {
        courseListDiv.innerHTML = '<p>No courses are currently available.</p>';
        return;
    }

    // --- grid header ---
    const header = document.createElement('div');
    header.classList.add('course-grid-header');
    header.innerHTML = `
        <div>Course ID</div>
        <div>Course Title</div>
        <div>Capacity</div>
        <div>Enrolled</div>
        <div>Status</div>
        <div>Action</div>
    `;
    courseListDiv.appendChild(header);

    // --- course rows ---
    courses.forEach((course, index) => {
        const courseElement = document.createElement('div');
        courseElement.classList.add('course-item');

        const statusClass = course.isOpen ? 'open' : 'closed';
        const statusText = course.isOpen ? 'Open' : 'Closed';

        courseElement.innerHTML = `
        <div class="course-id" data-label="Course Code">${course.id}</div>
        <div class="course-title" data-label="Course Title">${course.title}</div>
        <div class="course-capacity" data-label="Capacity">${course.capacity}</div>
        <div class="course-enrolled" data-label="Enrolled">${course.enrolled}</div>
        <div class="course-status" data-label="Status">
            <span class="status ${statusClass}">${statusText}</span>
        </div>
        <div class="course-action" data-label="Action">
            ${course.isOpen ?
                `<button onclick="enrollInCourse('${course.id}')">Enroll</button>` :
                '<button disabled>Closed</button>'
            }
        </div>
    `;
        courseListDiv.appendChild(courseElement);
    });
}

async function enrollInCourse(courseId) {
    enrollMessage.style.display = 'none';

    try {
        const response = await fetch('http://localhost:8082/api/enroll', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${localStorage.getItem('authToken')}`
            },
            body: JSON.stringify({ courseId: courseId })
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.error || `Enrollment failed: ${response.status}`);
        }

        const result = await response.json();
        showEnrollMessage(result.message, 'success');

        fetchCourses();

    } catch (error) {
        console.error('Enrollment error:', error);
        showEnrollMessage('Enrolling is currently unavailable. Please try again later.', 'error');
    }
}

function showEnrollMessage(message, type) {
    enrollMessage.textContent = message;
    enrollMessage.className = '';
    enrollMessage.classList.add(type);
    enrollMessage.style.display = 'block';

    setTimeout(() => {
        enrollMessage.style.display = 'none';
    }, 4000);
}