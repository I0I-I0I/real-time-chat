import { useState } from "react"

import { Login } from "@/components"
import { Gradient } from "@/components/UI"
import { redirect } from "react-router-dom"

type CurrentStateType = "sing-in" | "sing-up"

const LoginPage = () => {
	const [currentState, setCurrentState] = useState<CurrentStateType>("sing-in")

	const onLinkClick = (e: React.MouseEvent<HTMLAnchorElement, MouseEvent>): void => {
		e.preventDefault()
		setCurrentState(currentState === "sing-in" ? "sing-up" : "sing-in")
	}

	const onSubmit: React.FormEventHandler<HTMLButtonElement> = (e) => {
		e.preventDefault()
		redirect("/chat")
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
