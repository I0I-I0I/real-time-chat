import { Input, Modal, Button, Link, Typography } from "@/components/UI"

import styles from "./Login.module.css"
import { IPostUser } from "@/types"
import useInput from "@/hooks/useInput"

interface LoginProps {
	currentState: string
	onLinkClick: (event: React.MouseEvent<HTMLAnchorElement, MouseEvent>) => void
	onSubmit: (event: React.FormEvent<HTMLFormElement>, data: IPostUser) => void
}

export const Login = ({
	currentState,
	onLinkClick,
	onSubmit: onSubmit
}: LoginProps): JSX.Element => {
	const [login_prop,] = useInput("")
	const [username_prop,] = useInput("")
	const [password_prop,] = useInput("")

	const handleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
		onSubmit(e, {
			login: login_prop.value,
			username: username_prop.value,
			password: password_prop.value
		})
	}

	return (
		<>
			<Modal className={styles.modal_login}>
				<Typography tag="h1" variant="title-1">
					{ currentState === "sing-in" ? "Sing in" : "Sing Up" }
				</Typography>
				<form action="POST" className="form sing-in" onSubmit={handleSubmit}>
					<Input
						className="input login__input"
						type="text"
						placeholder="Login..."
						{...login_prop}
					/>
					{
						currentState === "sing-up" &&
						<Input
							className="input login__input"
							type="text"
							placeholder="Username..."
							{...username_prop}
						/>
					}
					<Input
						className="input login__input"
						type="password"
						placeholder="Password..."
						{...password_prop}
					/>
					<Button className={styles.button} type="submit">
						{ currentState === "sing-in" ? "Sing in" : "Sing Up" }
					</Button>
					<Link href="#" onClick={onLinkClick} className={styles.link}>
						{ currentState === "sing-in" ? "or sing up" : "or sing in" }
					</Link>
					<Link href="/chat" className={styles.link}>
						Go to chat
					</Link>
				</form>
			</Modal>
		</>
	)
}
