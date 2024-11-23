import { useState } from "react"

import { Login } from "@/components"
import { Gradient } from "@/components/UI"
import { redirect } from "react-router-dom"

import UserService from "@/api/UserService"
import { IPostUser } from "@/types"

type CurrentStateType = "sing-in" | "sing-up"

const LoginPage = () => {
	const [currentState, setCurrentState] = useState<CurrentStateType>("sing-in")

	const onLinkClick = (e: React.MouseEvent<HTMLAnchorElement, MouseEvent>): void => {
		e.preventDefault()
		setCurrentState(currentState === "sing-in" ? "sing-up" : "sing-in")
	}

	const onSubmit = (
		e: React.FormEvent<HTMLFormElement>,
		data: IPostUser
	) => {
		e.preventDefault()

		if (currentState === "sing-up") {
			UserService.createOne(data)
			.then((user) => {
				if (user) {
					redirect("/chat")
				}
			})
		} else if (currentState === "sing-in") {
			UserService.checkOne({
				login: data.login,
				password: data.password
			})
			.then((user) => {
				if (user) {
					redirect("/chat")
				}
			})
		}
	}

	return (
		<>
			<Gradient />
			<Login
				currentState={currentState}
				onLinkClick={onLinkClick}
				onSubmit={onSubmit}
			/>
		</>
	)
}

export default LoginPage
