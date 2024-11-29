// Update intervals (in milliseconds)
const STATUS_INTERVAL = 5000;  // 5 seconds
const VALUE_INTERVAL = 1000;   // 1 second

// Helper function for API calls
async function fetchAPI(endpoint) {
    try {
        const response = await fetch(endpoint);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return await response.json();
    } catch (error) {
        console.error('API call failed:', error);
        return null;
    }
}

// Update system status
async function updateStatus() {
    const data = await fetchAPI('/api/v1/status');
    if (data) {
        document.getElementById('uptime').textContent = data.uptime;
        document.getElementById('heap').textContent = data.heap;
        document.getElementById('buildTime').textContent = data.build_time;
    }
}

// Update KNX value
async function updateValue() {
    const data = await fetchAPI('/api/v1/knx/value');
    if (data) {
        document.getElementById('value').textContent = data.value.toFixed(1);
    }
}

// Initialize periodic updates
function startUpdates() {
    // Initial updates
    updateStatus();
    updateValue();
    
    // Set up intervals
    setInterval(updateStatus, STATUS_INTERVAL);
    setInterval(updateValue, VALUE_INTERVAL);
}

// Start when page loads
document.addEventListener('DOMContentLoaded', startUpdates);

// Handle visibility changes
document.addEventListener('visibilitychange', () => {
    if (document.visibilityState === 'visible') {
        // Immediate update when page becomes visible
        updateStatus();
        updateValue();
    }
});
