FROM nginx:alpine
# Copy all frontend files to the correct Nginx directory
COPY frontend /usr/share/nginx/html
# Copy Nginx configuration
COPY default.conf /etc/nginx/conf.d/default.conf

# Expose port 80
EXPOSE 80