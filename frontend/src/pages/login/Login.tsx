import { useState } from "react"

import { Login } from "@/components"
import { Gradient } from "@/components/UI"
import styles from "./Login.module.css"

type CurrentStateType = "sing-in" | "sing-up"

const LoginPage = () => {
	const [currentState, setCurrentState] = useState<CurrentStateType>("sing-in")

	const onLinkClick = (e: React.MouseEvent<HTMLAnchorElement, MouseEvent>): void => {
		e.preventDefault()
		setCurrentState(currentState === "sing-in" ? "sing-up" : "sing-in")
	}

	return (
		<>
			<Gradient />
			<Login
				currentState={currentState}
				onLinkClick={onLinkClick}
			/>
		</>
	)
}

export default LoginPage
