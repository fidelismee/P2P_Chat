const ws = new WebSocket("ws://localhost:9002");

ws.onopen = () => {
    console.log("Connected to WebSocket server");
};

ws.onmessage = (event) => {
    console.log("Received:", event.data);
    displayMessage(event.data);
};

ws.onerror = (error) => {
    console.error("WebSocket Error:", error);
};

ws.onclose = () => {
    console.log("WebSocket connection closed");
};

document.getElementById("sendBtn").addEventListener("click", sendMessage);
document.getElementById("messageInput").addEventListener("keypress", (event) => {
    if (event.key === "Enter") sendMessage();
});

function sendMessage() {
    const input = document.getElementById("messageInput");
    const message = input.value.trim();
    if (message !== "") {
        ws.send(message);
        displayMessage(`You: ${message}`);
        input.value = "";
    }
}

function displayMessage(message) {
    const messagesDiv = document.getElementById("messages");
    const messageElement = document.createElement("p");
    messageElement.textContent = message;
    messagesDiv.appendChild(messageElement);
    messagesDiv.scrollTop = messagesDiv.scrollHeight; // Auto-scroll to the latest message
}
