# KNX Web Server Example

This example demonstrates how to use the built-in web server component of the KNX Platform IO library.

## Features Demonstrated

1. File Serving
- Automatic serving of static files from LittleFS
- Proper MIME type handling
- Chunked transfer for large files
- Cache control using build timestamp

2. API Endpoints
- Custom API endpoint creation
- JSON response handling
- System status information
- KNX value access

3. Web Interface
- Responsive design
- Real-time updates
- Clean error handling
- Visibility state management

## Usage

1. Upload the files in the `data` directory to LittleFS:
   - index.html
   - style.css
   - app.js

2. Build and upload the firmware

3. Access the web interface:
   - Open a browser and navigate to `http://<device-ip>/`
   - The status page will show system information and KNX values
   - API endpoints are available at:
     * `/api/v1/status` - System status
     * `/api/v1/knx/value` - Current KNX value

## Disabling Web Server

To disable the web server component, define `NOWEB` in your build flags:

```ini
build_flags = 
    -DNOWEB
```

## API Documentation

### GET /api/v1/status
Returns system status information:
```json
{
    "uptime": 123,
    "heap": 45678,
    "build_time": "Dec 31 2023 12:34:56"
}
```

### GET /api/v1/knx/value
Returns the current KNX value:
```json
{
    "value": 19.8
}
```

## File Structure

```
data/
├── index.html    # Main web interface
├── style.css     # Styling
└── app.js        # Client-side logic

src/
└── knxapp.cpp    # Application code
```

## Notes

- The web server uses build timestamp for cache control
- Files are served in chunks to manage memory usage
- API endpoints return JSON responses
- The web interface updates automatically
