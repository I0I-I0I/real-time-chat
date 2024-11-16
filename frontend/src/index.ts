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

interface TestData {
	login: string
	username: string | undefined
	password: string
}

async function request_to_server(method: string, id: string, data: TestData[]) {
	try {
		const json = JSON.stringify(data)
		console.log("Sended: ", json)
		const response = await fetch("http://localhost:8080/db/users?id=" + id, {
			method: method,
			body: json
		})
		const message = await response.json()
		return message
	} catch (error) {
		console.error("Failed to fetch from server:", error)
	}
}

const username_input = document.getElementById("usernameInput") as HTMLInputElement
const login_input = document.getElementById("LoginInput") as HTMLInputElement
const password_input = document.getElementById("passwordInput") as HTMLInputElement

const method_input = document.getElementById("methodInput") as HTMLInputElement
const body_input = document.getElementById("bodyInput") as HTMLInputElement
const get_data_button = document.getElementById("connectButton") as HTMLButtonElement
const send_data_button = document.getElementById("sendButton") as HTMLButtonElement

get_data_button.addEventListener("click", async () => {
	const value = await get_from_server(body_input.value)
	console.log(value)
})

send_data_button.addEventListener("click", async () => {
	const value = await request_to_server(method_input.value, body_input.value, [{
		username: (username_input.value != "" ? username_input.value : undefined),
		login: login_input.value,
		password: password_input.value
	}])
	console.log(value)
})
