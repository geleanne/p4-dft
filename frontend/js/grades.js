document.addEventListener('DOMContentLoaded', function() {
    const gradesListDiv = document.getElementById('grades-list');

    // fetch and display grades
    async function fetchAndDisplayGrades() {
        try {
            const response = await fetch('http://localhost:8081/api/grades', {
                method: 'GET',
                headers: {
                    'Authorization': `Bearer ${localStorage.getItem('authToken')}`
                }
            });

            if (!response.ok) {
                const message = `An error occurred: ${response.status}`;
                throw new Error(message);
            }

            const grades = await response.json();
            if (grades.length > 0) {
                let gradesHTML = `
                    <table>
                        <thead>
                            <tr>
                                <th>Course Code</th>
                                <th>Course Title</th>
                                <th>Grade</th>
                                <th>Semester</th>
                                <th>Year</th>
                            </tr>
                        </thead>
                        <tbody>
                `;
                grades.forEach(grade => {
                    gradesHTML += `
                            <tr>
                                <td>${grade.courseCode}</td>
                                <td>${grade.courseTitle}</td>
                                <td>${grade.finalGrade}</td>
                                <td>${grade.semester}</td>
                                <td>${grade.academicYear}</td>
                            </tr>
                    `;
                });
                gradesHTML += `
                        </tbody>
                    </table>
                `;
                gradesListDiv.innerHTML = gradesHTML;
            } else {
                gradesListDiv.innerHTML = '<p>No grades available yet.</p>';
            }

        } catch (error) {
            console.error('Error fetching grades:', error);
            gradesListDiv.innerHTML = `<div class="error-message">Failed to load grades. Please try again later.</div>`;
        }
    }

    fetchAndDisplayGrades();
});