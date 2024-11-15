async function get_from_server(msg: string) {
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

async function post_to_server(data: object[]) {
	try {
		const json = JSON.stringify(data)
		console.log("Sended: ", json)
		const response = await fetch("http://localhost:8080/db/users", {
			method: "POST",
			body: json
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
	const value = await get_from_server(messageInput.value)
	console.log(value)
})

sendButton.addEventListener("click", async () => {
	const value = await post_to_server([{
		username: messageInput.value,
		login: messageInput.value,
		password: "123"
	}])
	console.log(value)
})
