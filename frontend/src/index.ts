async function pollServer() {
	try {
		// Make an HTTP GET request to the C++ server
		const response = await fetch("http://localhost:8080")

		// Display server response
		const message = await response.text()
		console.log(message)

		// Poll again after receiving the response
		setTimeout(pollServer, 1000) // Adjust the delay as needed
	} catch (error) {
		console.error("Failed to fetch from server:", error)
		setTimeout(pollServer, 3000) // Retry after a delay on error
	}
}

const messageInput = document.getElementById("messageInput") as HTMLInputElement
const connectButton = document.getElementById("connectButton") as HTMLButtonElement
const sendButton = document.getElementById("sendButton") as HTMLButtonElement

connectButton.addEventListener("click", async () => {
	pollServer()
})

