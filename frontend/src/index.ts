async function pollServer() {
	try {
		const response = await fetch("http://localhost:8080")

		const message = await response.text()
		console.log(message)
	} catch (error) {
		console.error("Failed to fetch from server:", error)
	}
}

const messageInput = document.getElementById("messageInput") as HTMLInputElement
const connectButton = document.getElementById("connectButton") as HTMLButtonElement
const sendButton = document.getElementById("sendButton") as HTMLButtonElement

connectButton.addEventListener("click", async () => {
	pollServer()
})
