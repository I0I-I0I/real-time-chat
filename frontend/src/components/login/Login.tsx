import { useState } from "react"

const Login = (): JSX.Element => {
	const [currentState, setCurrentState] = useState("login")

	const onLinkClick = (e: React.MouseEvent<HTMLAnchorElement, MouseEvent>): void => {
		e.preventDefault()
		setCurrentState("login")
	}

	return (
		<section className="modal modal_login">
			<h1 className="title-1">
			{
				"Sing Up"
			}
			</h1>
			<form action="POST" className="form login">
				<input className="input login__input" type="text" placeholder="Login..." />
				{
					currentState === "sing-up" &&
					<input className="input login__input" type="text" placeholder="Username..." />
				}
				<input className="input login__input" type="password" placeholder="Password..." />
				<button className="button login__button button__default" type="submit">
					{ currentState === "login" ? "Sing In" : "Sing Up" }
				</button>
				<a href="./login.html" onClick={onLinkClick} className="link">
					{ currentState === "login" ? "or sing in" : "or sing up" }
				</a>
			</form>
		</section>
	)
}

export default Login
