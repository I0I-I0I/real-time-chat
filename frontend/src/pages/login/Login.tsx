import { useState } from "react"

import { Login } from "@/components"
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
			<div className={styles.gradient}></div>
			<Login
				currentState={currentState}
				onLinkClick={onLinkClick}
			/>
		</>
	)
}

export default LoginPage
