async function pollServer(msg: string) {
	try {
		const response = await fetch("http://localhost:8080/db/users" + (msg ? `?id=${msg}` : ""), {
			method: "GET"
		})
		const message = await response.json()
		return message
	} catch (error) {
		console.error("Failed to fetch from server:", error)
	}
}

const messageInput = document.getElementById("messageInput") as HTMLInputElement
const connectButton = document.getElementById("connectButton") as HTMLButtonElement
const sendButton = document.getElementById("sendButton") as HTMLButtonElement

connectButton.addEventListener("click", async () => {
	const value = await pollServer(messageInput.value)
	for (const user of value) {
		console.log(user.username)
	}
})
