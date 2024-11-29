document.addEventListener('DOMContentLoaded', () => {
    const temperatureValue = document.querySelector('.value');
    const connectionStatus = document.querySelector('.connection-status');
    let ws = null;
    let reconnectAttempts = 0;
    const maxReconnectAttempts = 5;

    function connect() {
        // Use secure WebSocket if page is served over HTTPS
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.hostname}:81`;
        
        ws = new WebSocket(wsUrl);

        ws.onopen = () => {
            connectionStatus.textContent = 'Connected';
            connectionStatus.classList.add('connected');
            connectionStatus.classList.remove('disconnected');
            reconnectAttempts = 0;
        };

        ws.onclose = () => {
            connectionStatus.textContent = 'Disconnected';
            connectionStatus.classList.add('disconnected');
            connectionStatus.classList.remove('connected');
            
            // Attempt to reconnect
            if (reconnectAttempts < maxReconnectAttempts) {
                reconnectAttempts++;
                connectionStatus.textContent = `Reconnecting (${reconnectAttempts}/${maxReconnectAttempts})...`;
                setTimeout(connect, 3000);
            } else {
                connectionStatus.textContent = 'Connection failed. Please refresh the page.';
            }
        };

        ws.onerror = (error) => {
            console.error('WebSocket error:', error);
        };

        ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                if (data.temperature !== undefined) {
                    temperatureValue.textContent = data.temperature.toFixed(1);
                }
            } catch (e) {
                console.error('Error parsing WebSocket message:', e);
            }
        };
    }

    // Initial connection
    connect();
});
