import { useState } from "react"
import { useNavigate } from "react-router-dom"

import UserService from "@/api/UserService"
import { IPostUser } from "@/types"

import { Login } from "@/components"
import { Gradient } from "@/components/UI"

type CurrentStateType = "sing-in" | "sing-up"

const LoginPage = () => {
	const navigate = useNavigate()
	const [currentState, setCurrentState] = useState<CurrentStateType>("sing-in")

	const onLinkClick = (e: React.MouseEvent<HTMLAnchorElement, MouseEvent>): void => {
		e.preventDefault()
		setCurrentState(currentState === "sing-in" ? "sing-up" : "sing-in")
	}

	const onSubmit = async (
		e: React.FormEvent<HTMLFormElement>,
		data: IPostUser
	): Promise<void> => {
		e.preventDefault()

		if (currentState === "sing-up") {
			const status = await UserService.createOne(data)
			if (status) navigate("/chat")
		} else if (currentState === "sing-in") {
			const status = await UserService.checkOne({
				login: data.login,
				password: data.password
			})
			if (status) navigate("/chat")
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
